//
// Created by daniel on 19/08/21.
//

#include "mkfs.h"

mkfs::mkfs(vector<string> parametros, mount montadas) {
    for (string param : parametros){
        bool encontrado = false;
        for (int i = 0; i < param.length(); ++i) {
            if (param[i] == '='){
                encontrado = true;
                break;
            }
        }
        if (!encontrado){
            cout << "-- ERROR PARAMETRO NO ASIGNADO --" << endl;
            return;
        }
        stringstream input_stringstream(param);
        string name, info;
        getline(input_stringstream, name, '=');
        getline(input_stringstream, info, '=');
        name = ToLower(name);
        info = ToLower(info);
        if (name == "-id"){
            this->id = info;
        }else if(name == "-type"){
            this->type = info;
        }else if(name == "-fs"){
            this->fs = info;
        }else{
            cout << " PARAMAETRO NO ENCONTRADO EN EL COMANDO MKFS" << endl;
            return;
        }
    }
    if (this->id.empty()){
        cout << "-- NO SE HA COLOCADO NINGUN ID EN EL COMANDO --" << endl;
        return;
    }
    if (this->type.empty()){
        this->type = "full";
    }
    if (this->fs.empty()){
        this->fs = "2fs";
    }
    this->montadas = montadas;
    this->particionMontada();
}

void mkfs::particionMontada() {
    bool sia = false;
    int i;
    string name;
    for (i = 0; i < 10; ++i) {
        if (montadas.discos[i].estado == 1){
            for (int j = 0; j < 24; ++j) {
                if (montadas.discos[i].particiones[j].estado == 1){
                    if (montadas.discos[i].particiones[j].id == this->id){
                        sia = true;
                        name = montadas.discos[i].particiones[j].nombre;
                        cout << "-- FORMATEANDO PARTICION: " << montadas.discos[i].particiones[j].nombre << " CON ID: " << montadas.discos[i].particiones[j].id << endl << endl;
                        break;
                    }
                }
            }
        }
        if (sia){
            break;
        }
    }
    if (!sia){
        cout << "-- NO SE ENCONTRO LA PARTICION MONTADA --" << endl;
        return;
    }
    FILE *file;
    file = fopen(montadas.discos[i].path.c_str(), "rb+"); // Se abre con permisos mixtos
    if (file == NULL) {
        cout << endl <<"----ERROR: EL DISCO NO EXISTE-----" << endl;
        return;
    }
    mbr MBR;
    fseek(file, 0, SEEK_SET);
    fread(&MBR, sizeof(mbr), 1, file); // Se recupera lo que hay en el archivo en MBR, del tamaño del struct mbr
    //------------------------
    ebr aux;
    particion auxp;
    bool otra;
    bool pe = false;
    bool l = false;
    for (int j = 0; j < 4; ++j) {
        if (MBR.mbr_partitions[j].part_name == name){
            pe = true;
            auxp = MBR.mbr_partitions[j];
            break;
        }
        if (MBR.mbr_partitions[j].part_type == 'e'){
            fseek(file, MBR.mbr_partitions[j].part_start, SEEK_SET);
            fread(&aux, sizeof(ebr), 1, file);
            do{
                if(aux.part_name == name){
                    l = true;
                    auxp = MBR.mbr_partitions[j];
                    break;
                }
                if (aux.part_next > 0){
                    fseek(file, aux.part_next, SEEK_SET);
                    fread(&aux, sizeof(ebr), 1, file);
                    otra = true;
                }else{
                    otra = false;
                }
            }while(otra);
        }
        if (l){
            break;
        }
    }
    this->formatear(file, MBR, auxp, aux, pe);
}

void mkfs::formatear(FILE* file, mbr temp, particion auxp, ebr aux, bool primaria) {
   int n = 0;
   int tp, start, tipo;
    if (primaria){
        tp = auxp.part_size;
        start = auxp.part_start;
    }else{
        tp = aux.part_size;
        start = aux.part_start;
    }
    if (this->fs == "2fs"){
        tipo = 2;
        n = (tp - sizeof (superbloque))/(4 + sizeof(inodo) + 3*sizeof(bloque_archivos)); // tp size particion
    }else if(this->fs == "3fs"){
        tipo = 3;
        n = (tp - sizeof(superbloque) - 100*sizeof (bloque_archivos))/(4 + sizeof(inodo) + 3*sizeof(bloque_archivos));
    }else{
        cout << "-- TIPO DE FORMATEO INCORRECTO --" << endl;
        return;
    }
    //n = floor(n);
    time_t t;
    struct tm *tm;
    char fechayhora[16];
    t = time(NULL);
    tm = localtime(&t);
    strftime(fechayhora, 16, "%d/%m/%Y %H:%M:%S", tm);
    fseek(file, start, SEEK_SET);
    if(this->type == "full"){
        char buff;
        buff='0';
        for(int i=0; i < tp; i++) { //lleno de ceros el archivo
            fwrite(&buff, sizeof(buff) , 1, file);
            fseek(file, start + i, SEEK_SET);
        }
    }
    superbloque sbloque;
    sbloque.s_filesystem_type = tipo;
    sbloque.s_inodes_count = n;
    sbloque.s_blocks_count = 3*n;
    sbloque.s_free_blocks_count = 3*n;//?
    sbloque.s_free_inodes_count = n; //?
    strcpy(sbloque.s_mtime, fechayhora);
    strcpy(sbloque.s_umtime, fechayhora);
    sbloque.s_mnt_count = 1;// revisar
    sbloque.s_magic=0xEF53; /// revisar
    sbloque.s_inode_size = sizeof(inodo);
    sbloque.s_block_size = sizeof(bloque_archivos);
    sbloque.s_first_ino = 2; //cuidado - el primer inodo libre va a ser el 2
    sbloque.s_first_blo = 2; // cuidado -
    if (tipo == 2) sbloque.s_bm_inode_start = start + sizeof(superbloque);
    if (tipo == 3) sbloque.s_bm_inode_start = start + sizeof(superbloque) + 100*sizeof(bloque_archivos);
    sbloque.s_bm_block_start = sbloque.s_bm_inode_start + n ;
    sbloque.s_inode_start = sbloque.s_bm_block_start+3*n; // cuidado
    sbloque.s_block_start = sbloque.s_inode_start+n*sizeof(inodo); // cuidado
    fseek(file, start, SEEK_SET);
    fwrite(&sbloque,sizeof(superbloque),1, file); //escribimos el superbloque en la particion

    char bitinodos[n]; //ahora creo los bitmap de inodos y bloques
    char bibloques[3*n];
    int iniciobitnodos = start + sizeof(superbloque);
    if (tipo == 3) iniciobitnodos += 100*sizeof(bloque_archivos);
    for(int i=2;i<n;i++){
        bitinodos[i]='0';
    }
    bitinodos[0]='1';
    bitinodos[1]='1';
    fseek(file, iniciobitnodos, SEEK_SET);
    fwrite(&bitinodos, sizeof(bitinodos),1,file);
    int iniciobloques=iniciobitnodos+n; //-------escribo mi bitmap de bloques
    for(int i=2;i<3*n;i++){
        bibloques[i]='0';
    }
    bibloques[0]='1';//---1 para direnciar carpeta
    bibloques[1]='2';//--2 para diferenciar archivo
    fseek(file,iniciobloques, SEEK_SET);
    fwrite(&bibloques,sizeof(bibloques),1,file);//ver si es sizeof(bibloques)//o si es sizeof(char)

    superbloque auxsuperbloque;
    fseek(file,start,SEEK_SET);
    fread(&auxsuperbloque,sizeof(superbloque),1,file);
    inodo raiz;
    raiz.i_uid=1;
    raiz.i_gid=1;
    raiz.i_size=0;//REVISAR
    memset(raiz.i_atime,0,sizeof(raiz.i_atime));
    memset(raiz.i_mtime,0,sizeof(raiz.i_atime));
    memset(raiz.i_ctime,0,sizeof(raiz.i_atime));
    strcpy(raiz.i_atime, fechayhora);
    strcpy(raiz.i_mtime, fechayhora);
    strcpy(raiz.i_ctime, fechayhora);
    for(int i=0;i<15;i++){
        raiz.i_block[i]=-1;
    }
    raiz.i_type='0'; //es carpeta
    raiz.i_perm=664;
    raiz.i_block[0]=0;
    bloque_carpetas carpetaroot;
    content contenraiz;
    strcpy(contenraiz.b_name, ".");//definimos la carpeta actual
    contenraiz.b_inodo = 0; //apuntador
    carpetaroot.b_content[0] = contenraiz;
    strcpy(contenraiz.b_name, "..");//definimos al padre de la carpeta
    carpetaroot.b_content[1] = contenraiz;
    //content para archivo users.txt
    strcpy(contenraiz.b_name, "users.txt");//agregamos nombre
    contenraiz.b_inodo = 1;//apuntadora hacia inodo asociado
    carpetaroot.b_content[2] = contenraiz;//guardamos el inodo archivo a los apuntadores del inodo de la carpeta raiz
    memset(&contenraiz.b_name, 0, sizeof(contenraiz.b_name));//definimos la carpeta actual
    contenraiz.b_inodo = -1;
    carpetaroot.b_content[3] = contenraiz;
    fseek(file, auxsuperbloque.s_inode_start, SEEK_SET);
    fwrite(&raiz, sizeof(inodo), 1, file);
    auxsuperbloque.s_free_inodes_count--;//disminuyo en 1 los inodos ocupados
    fseek(file, auxsuperbloque.s_block_start, SEEK_SET);
    fwrite(&carpetaroot, 64, 1, file);
    auxsuperbloque.s_free_blocks_count--;//disminuyo en 1 los bloques de archivos ocupados
    char datosarchivo[]="1,G,root\n1,U,root,root,123\n";//cadena a quemar en el archivo
    inodo archivousuarios;
    archivousuarios.i_gid=1;
    archivousuarios.i_size=sizeof(datosarchivo);//el tamaño de la cadena que voy a quemar
    archivousuarios.i_uid=1;
    memset(archivousuarios.i_atime,0,sizeof(archivousuarios.i_atime));
    memset(archivousuarios.i_mtime,0,sizeof(archivousuarios.i_atime));
    memset(archivousuarios.i_ctime,0,sizeof(archivousuarios.i_atime));
    //archivousuarios.i_ctime[0]='00';
    strcpy(archivousuarios.i_atime, fechayhora);
    strcpy(archivousuarios.i_mtime, fechayhora);
    strcpy(archivousuarios.i_ctime, fechayhora);
    for(int i=0;i<15;i++){
        archivousuarios.i_block[i]=-1;
    }
    archivousuarios.i_perm=664;
    archivousuarios.i_block[0]=1;
    archivousuarios.i_type='1'; //es archivo
    //termino de crear el inodo de archivos usuario
    bloque_archivos bloquearchivos;
    strcpy(bloquearchivos.b_content,datosarchivo);
    fseek(file, auxsuperbloque.s_inode_start + sizeof(inodo), SEEK_SET);
    fwrite(&archivousuarios, sizeof(inodo), 1, file);
    auxsuperbloque.s_free_inodes_count--;
    fseek(file, auxsuperbloque.s_block_start+64, SEEK_SET);
    fwrite(&bloquearchivos, 64, 1, file);
    auxsuperbloque.s_free_blocks_count--;
    fseek(file, start, SEEK_SET);
    fwrite(&auxsuperbloque, sizeof(superbloque), 1, file);
    std::cout << "\nPARTICION FORMATEADA CORRECTAMENTE!!! \n";
    fclose(file);
}

string mkfs::ToLower(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}
