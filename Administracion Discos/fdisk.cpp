//
// Created by daniel on 12/08/21.
//

#include "fdisk.h"

fdisk::fdisk(vector<string> parametros) {
    for (string param : parametros){
        stringstream input_stringstream(param);
        string name, info;
        getline(input_stringstream, name, '=');
        getline(input_stringstream, info, '=');
        name = ToLower(name);
        if (name == "-path"){
            path = info;
        }
        info = ToLower(info);
        if(name == "-u"){
            u = info;
        }else if(name == "-size"){
            size = stoi(info);
        }else if(name == "-f"){
            fit = info;
        }else if(name == "-type"){
            type = info;
        }else if(name == "-delete"){
            del = info;
        }else if(name == "-name"){
            this->name = info;
        }else if(name == "-add"){
            add = info;
        }
    }
    if (!del.empty()){
        this->deleteParticion();
    }else if(!add.empty()){

    }else{
        this->CrearParticion();
    }
}

void fdisk::CrearParticion() {
    particion part;
    int numsize;
    if(this->u == "k"){ //kb
        numsize =  this->size * 1024;
    }else if(this->u == "b"){
        numsize = this->size;
    }else if(this->u == "m"){ //mb
        numsize =  this->size * 1024 * 1024;
    }else{
        cout << endl << "---UNIDADES DE SIZE INCORRECTAS---" << endl;
        return;
    }
    cout << numsize << endl;
    if (size < 0 ){
        cout << endl << "---ERRROR DE SIZE, DEBE SER MAYOR A CERO---" << endl;
        return;
    }
    char f;
    if(this->fit.empty()){
        f = 'w';
    }else if(this->fit[0] == 'b'){
        f = 'b';
    }else if(this->fit[0] == 'f'){
        f = 'f';
    }else if (this->fit[0] == 'w'){
        f = 'w';
    }else{
        cout << endl << "---- ERROR EN LA CONFIGURACION DE LA PARTICION ---" << endl;
        return;
    }
    char t;
    if(this->type.empty()){
        t = 'p';
    }else if(this->type[0] == 'e'){
        t = 'e';
    }else if(this->type[0] == 'p'){
        t = 'p';
    }else if (this->type[0] == 'l'){
        t = 'l';
    }else{
        cout << endl << "---- ERROR EN EL TIPO DE PARTICION ---" << endl;
        return;
    }
    if(this->name.empty()){
        cout << endl << "--- LA PARTICION TIENE QUE TENER NOMBRE --" << endl;
        return;
    }
    //CREO LA PARTICION
    part.part_status = '1';
    part.part_fit = f;
    strcpy(part.part_name, name.c_str());
    part.part_start = 0;
    part.part_size = numsize;
    part.part_type = t;
    //mando particion al disco
    this->validarDisco(part);
}
void fdisk::validarDisco(particion part) {
    //----------------- BUSCAR DISCO --------------
    FILE *file;
    file = fopen(this->path.c_str(), "rb+"); // Se abre con permisos mixtos
    if (file == NULL) {
        cout << endl <<"----ERROR: EL DISCO NO EXISTE-----" << endl;
        return;
    }
    mbr tempDisk;
    fseek(file, 0, SEEK_SET);
    fread(&tempDisk, sizeof(mbr), 1, file); // Se recupera lo que hay en el archivo en tempDISK, del tamaño del struct mbr
    //fclose(file);
    // VALIDACIONES
    int disco4Particiones = 0;
    int disponible = 0;
    bool existeExtendida = false;
    particion auxE;
    for (int i = 0; i < 4; ++i) {
        if (tempDisk.mbr_partitions[i].part_status != '0'){
            disco4Particiones++;
            if (tempDisk.mbr_partitions[i].part_type == 'e'){
                existeExtendida = true;
                auxE = tempDisk.mbr_partitions[i];
            }
            disponible += tempDisk.mbr_partitions[i].part_size;
        }
    }
    disponible = tempDisk.mbr_tamano - disponible;
    if (disco4Particiones >= 4 && part.part_type != 'l'){
        cout << endl << "-- EXISTEN YA EL MAXIMO DE PARTICIONES EN EL DISCO --";
        return;
    }
    if(tempDisk.mbr_tamano < part.part_size && part.part_type != 'l'){
        cout << endl << "-- PARTICION MAS GRANDE QUE EL DISCO -- " << endl;
        return;
    }
    if (disponible < part.part_size && part.part_type != 'l'){
        cout << endl << "-- EL DISCO NO TIENE ESPACIO DISPONIBLE PARA ESTA PARTICION --" << endl;
        return;
    }
    if (existeExtendida){
        if (part.part_type == 'e'){
            cout << endl << "-- YA EXISTE UNA PARTICION EXTENDIDA EN EL DISCO --" << endl;
            return;
        }else if (part.part_type == 'l'){
            ebr aux;
            fseek(file, auxE.part_start, SEEK_SET);
            fread(&aux, sizeof(ebr), 1, file);
            int contador = 0;
            if (part.part_size > auxE.part_size){
                cout << endl << "--- LA PARTICION LOGICA ES MAS GRANDE QUE LA EXTENDIDA ---" << endl;
                return;
            }
            while (aux.part_next != -1){
                contador += aux.part_size;
                fseek(file, aux.part_next, SEEK_SET);
                fread(&aux, sizeof(ebr), 1, file);
            }
            if (aux.part_size != -1){
                contador += aux.part_size;
                contador = auxE.part_size - contador;
                if (part.part_size > contador){
                    cout << endl << "--- YA NO HAY ESPACIO EN LA EXTENDIDA PARA ESTA PARTICION LOGICA ---" << endl;
                    return;
                }
            }
        }
    }else{
        if (part.part_type == 'l'){
            cout << endl << "-- NO EXISTE UNA PARTICION EXTENDIDA EN EL DISCO PARA UNA LOGICA -- " << endl;
            return;
        }
    }
    /// MANDO A COLOCAR PARTICION
    if (part.part_type == 'p'){
        particionPrimaria(file, part, tempDisk);
    }else if (part.part_type == 'e'){
        particionExtendida(file, part, tempDisk);
    }else if (part.part_type == 'l'){
        particionLogica(file, part, tempDisk, auxE);
    }
    this->printDisco(file);/// MANDO A VERIFICAR SI SE GUARDARON CAMBIOS EN EL DISCO
}

void fdisk::particionPrimaria(FILE *file, particion particionu, mbr Disk) {
    switch (fitex(Disk.disk_fit)) {
        case 1: // BEST FIT
            break;
        case 2: // FIRST FIT
            for (int i = 0; i < 4; ++i) {
                if (Disk.mbr_partitions[i].part_status == '0'){
                    if (i == 0){ //
                        particionu.part_start = sizeof(mbr);
                        Disk.mbr_partitions[i] = particionu;
                        break;
                    }else{
                        particionu.part_start = Disk.mbr_partitions[i - 1].part_start + Disk.mbr_partitions[i - 1].part_size;
                        Disk.mbr_partitions[i] = particionu;
                        break;
                    }
                }
            }
            break;
        case 3: //  WORST FIT
            break;
        default:
            break;
    }
    fseek(file, 0, SEEK_SET);
    fwrite(&Disk, sizeof(mbr), 1, file);
}

void fdisk::particionExtendida(FILE *file, particion particionu, mbr Disk) {
    switch (fitex(Disk.disk_fit)) {
        case 1: // BEST FIT
            break;
        case 2: // FIRST FIT
            for (int i = 0; i < 4; ++i) {
                if (Disk.mbr_partitions[i].part_status == '0'){
                    if (i == 0){ //
                        particionu.part_start = sizeof(mbr);
                        Disk.mbr_partitions[i] = particionu;
                        break;
                    }else{
                        particionu.part_start = Disk.mbr_partitions[i - 1].part_start + Disk.mbr_partitions[i - 1].part_size;
                        Disk.mbr_partitions[i] = particionu;
                        break;
                    }
                }
            }
            break;
        case 3: //  WORST FIT
            break;
        default:
            break;
    }
    //ACTUALIZO
    fseek(file, 0, SEEK_SET);
    fwrite(&Disk, sizeof(mbr), 1, file);
    //INICIO UN EBR VACIO
    ebr particionE;
    particionE.part_status = '0';
    particionE.part_fit = particionu.part_fit;
    particionE.part_size = -1; //indicador
    particionE.part_name[0] = '\0';
    particionE.part_next = -1;
    particionE.part_start = particionu.part_start + sizeof(ebr);
    fseek(file, particionu.part_start, SEEK_SET);
    fwrite(&particionE, sizeof(ebr), 1, file); /// coloco al inicio de la extendida recien creada
}

void fdisk::particionLogica(FILE *file, particion particionu, mbr Disk, particion Extendida) {
    ebr aux;
    ebr particionE;
    particionE.part_status = particionu.part_status;
    particionE.part_fit = particionu.part_fit;
    particionE.part_size = particionu.part_size;
    strcpy(particionE.part_name, particionu.part_name);
    /////////////
    fseek(file, Extendida.part_start, SEEK_SET);
    fread(&aux, sizeof(ebr), 1, file);
    switch (fitex(Extendida.part_fit)) {
        case 1:
            break;
        case 2:
            particionE.part_next = -1;
            while(aux.part_next != -1){
                fseek(file, aux.part_next, SEEK_SET);
                fread(&aux, sizeof(ebr), 1, file);
            }
            if (aux.part_size == -1){
                particionE.part_start = aux.part_start;
                fseek(file, Extendida.part_start, SEEK_SET);
                fwrite(&particionE, sizeof(ebr), 1, file);
            }else{
                particionE.part_start = aux.part_start + aux.part_size + sizeof(ebr);
                aux.part_next = aux.part_start + aux.part_size;
                //creo el nuevo ebr
                fseek(file, aux.part_next, SEEK_SET);
                fwrite(&particionE, sizeof(ebr), 1, file);
                //actuzalizo
                fseek(file, aux.part_start - sizeof(ebr), SEEK_SET);
                fwrite(&aux, sizeof(ebr), 1, file);
            }
            break;
        case 3:
            break;
        default:
            break;
    }
}

void fdisk::deleteParticion() {
    FILE *file;
    file = fopen(this->path.c_str(), "rb+"); // Se abre con permisos mixtos
    if (file == NULL) {
        cout << endl <<"----ERROR: EL DISCO NO EXISTE-----" << endl;
        return;
    }
    mbr MBR;
    fseek(file, 0, SEEK_SET);
    fread(&MBR, sizeof(mbr), 1, file); // Se recupera lo que hay en el archivo en MBR, del tamaño del struct mbr
    ebr aux;
    particion auxp;
    bool otra;
    bool pe = false;
    bool l = false;
    for (int i = 0; i < 4; ++i) {
        if (MBR.mbr_partitions[i].part_name == this->name){
            pe = true;
            auxp = MBR.mbr_partitions[i];
            break;
        }
        if (MBR.mbr_partitions[i].part_type == 'e'){
            fseek(file, MBR.mbr_partitions[i].part_start, SEEK_SET);
            fread(&aux, sizeof(ebr), 1, file);
            do{
                if(aux.part_name == this->name){
                    l = true;
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
    if (!pe && !l){
        cout << endl << "-- NO EXISTE LA PARTICION EN EL DISCO --" << endl ;
        return;
    }
    string respuesta;
    while(respuesta != "s" || respuesta != "n"){
        cout << "Seguro que deseas eliminar la particion? [s/n]" << endl;
        cin >> respuesta;
    }
    if (respuesta == "n"){
        cout << "-- ACCION DEL COMANDO CANCELADO --" << endl;
        return;
    }
    if (l){ // si la particion es logica

    }
    if(pe){ // si la particion es primario o extendida

    }
}

void fdisk::printDisco(FILE* arch) {
    mbr MBR;
    ebr aux;
    fseek(arch, 0, SEEK_SET);
    fread(&MBR,sizeof(mbr),1,arch);
    bool otra = false;
    cout << "-----------------------------DATOS DEL DISCO--------------------------------" << endl;
    cout << "MBR SIGNATURE: "<< MBR.mbr_disk_signature << endl;
    cout << "MBR SIZE: "<< MBR.mbr_tamano << endl;
    cout << "FECHA: "<< MBR.mbr_fecha_creacion << endl;
    cout << "FIT: " << MBR.disk_fit << endl;
    for (int i = 0; i < 4; ++i) {
        cout << "PARTICION : "<< i + 1 << endl;
        cout << "PARTICION STATUS : "<< MBR.mbr_partitions[i].part_status << endl;
        cout << "PARTICION TYPE : "<< MBR.mbr_partitions[i].part_type << endl;
        cout << "PARTICION FIT : "<< MBR.mbr_partitions[i].part_fit << endl;
        cout << "PARTICION START : "<< MBR.mbr_partitions[i].part_start << endl;
        cout << "PARTICION SIZE : "<< MBR.mbr_partitions[i].part_size << endl;
        cout << "PARTICION NAME : "<< MBR.mbr_partitions[i].part_name << endl;
        if (MBR.mbr_partitions[i].part_type == 'e'){
            fseek(arch, MBR.mbr_partitions[i].part_start, SEEK_SET);
            fread(&aux, sizeof(ebr), 1, arch);
            cout << "---------------------" << endl;
            cout << "EBR" << endl;
            do{
                cout << "\tNAME " << aux.part_name << endl;
                cout << "\tSIZE " << aux.part_size << endl;
                cout << "\tNEXT " << aux.part_next << endl;
                if (aux.part_next > 0){
                    fseek(arch, aux.part_next, SEEK_SET);
                    fread(&aux, sizeof(ebr), 1, arch);
                    otra = true;
                    cout << "--" << endl;
                }else{
                    otra = false;
                }
            }while(otra);
            cout << "----------------------";
        }
        cout << endl;
    }
    fclose(arch);
}

int fdisk::fitex(char fit) {
    if (fit == 'b')
        return 1;
    else if(fit == 'f')
        return 2;
    else if (fit == 'w')
        return 3;
    return 0;
}

string fdisk::ToLower(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}



