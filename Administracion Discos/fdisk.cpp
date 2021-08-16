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
    //-----------------------------------------
    if (size < 0 ){
        cout << endl << "---ERRROR DE SIZE, DEBE SER MAYOR A CERO---" << endl;
        return;
    }
    //---------------------------------------
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
    //----------------------------
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
    for (int i = 0; i < 4; ++i) {
        if (tempDisk.mbr_partitions[i].part_status != '0'){
            disco4Particiones++;
            if (tempDisk.mbr_partitions[i].part_type == 'e'){
                existeExtendida = true;
            }
            disponible += tempDisk.mbr_partitions[i].part_size;
        }
    }
    disponible = tempDisk.mbr_tamano - disponible;
    if (disco4Particiones >= 4){
        cout << endl << "-- EXISTEN YA EL MAXIMO DE PARTICIONES EN EL DISCO --";
        return;
    }
    if(tempDisk.mbr_tamano < part.part_size){
        cout << endl << "-- PARTICION MAS GRANDE QUE EL DISCO -- " << endl;
        return;
    }
    if (disponible < part.part_size){
        cout << endl << "-- EL DISCO NO TIENE ESPACIO DISPONIBLE PARA ESTA PARTICION --" << endl;
        return;
    }
    if (existeExtendida){
        if (part.part_type == 'e'){
            cout << endl << "-- YA EXISTE UNA PARTICION EXTENDIDA EN EL DISCO --" << endl;
            return;
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
        particionLogica(file, part, tempDisk);
    }

    /// MANDO A VERIFICAR SI SE GUARDARON CAMBIOS EN EL DISCO
    this->printDisco(file);
}

void fdisk::particionPrimaria(FILE *file, particion particionu, mbr Disk) {
    switch (fitDisk(Disk)) {
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
    rewind(file);
    fwrite(&Disk, sizeof(mbr), 1, file);
}

void fdisk::particionExtendida(FILE *file, particion particionu, mbr Disk) {
    switch (fitDisk(Disk)) {
        case 1: // BEST FIT
            break;
        case 2: // FIRST FIT
            break;
        case 3: //  WORST FIT
            break;
        default:
            break;
    }
}

void fdisk::particionLogica(FILE *file, particion particionu, mbr Disk) {
    ebr particionE;
    particionE.part_status = particionu.part_status;
    particionE.part_fit = particionu.part_fit;
    particionE.part_start = particionu.part_start;
    particionE.part_size = particionu.part_size;
    strcpy(particionE.part_name, particionu.part_name);
}


void fdisk::printDisco(FILE* arch) {
    mbr MBR;
    fseek(arch, 0, SEEK_SET);
    fread(&MBR,sizeof(mbr),1,arch);
    fclose(arch);
    cout << "-----------------------------DATOS DEL DISCO--------------------------------" << endl;
    cout << "MBR SIGNATURE: "<< MBR.mbr_disk_signature << endl;
    cout << "MBR SIZE: "<< MBR.mbr_tamano << endl;
    cout << "FECHA: "<< MBR.mbr_fecha_creacion << endl;
    cout << "FIT: " << MBR.disk_fit << endl;
    for(int i=0;i<4;i++){
        cout << "PARTICION : "<< i + 1 << endl;
        cout << "PARTICION STATUS : "<< MBR.mbr_partitions[i].part_status << endl;
        cout << "PARTICION TYPE : "<< MBR.mbr_partitions[i].part_type << endl;
        cout << "PARTICION FIT : "<< MBR.mbr_partitions[i].part_fit << endl;
        cout << "PARTICION START : "<< MBR.mbr_partitions[i].part_start << endl;
        cout << "PARTICION SIZE : "<< MBR.mbr_partitions[i].part_size << endl;
        cout << "PARTICION NAME : "<< MBR.mbr_partitions[i].part_name << endl;
        cout << endl;
    }
}

string fdisk::ToLower(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}

int fdisk::fitDisk(mbr Disk) {
    if (Disk.disk_fit == 'b')
        return 1;
    else if(Disk.disk_fit == 'f')
        return 2;
    else if (Disk.disk_fit == 'w')
        return 3;
    return 0;
}



