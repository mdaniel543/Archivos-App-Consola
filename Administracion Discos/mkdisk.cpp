//
// Created by daniel on 5/08/21.
//


#include "mkdisk.h"

mkdisk::mkdisk(vector<string> parametros) {
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
        if (name == "-path"){
            path = info;
            continue;
        }
        info = ToLower(info);
        if(name == "-u"){
            u = info;
        }else if(name == "-size"){
            size = stoi(info);
        }else if(name == "-f"){
            f = info;
        }else{
            cout << " PARAMAETRO NO ENCONTRADO EN EL COMANDO MKDISK " << endl;
            return;
        }
    }
    this->crearDisco();
}

void mkdisk::crearDisco() {
    mbr ine;
    this->CrearRuta();
    FILE  *archivo = fopen(path.c_str(), "r");
    if(archivo != NULL){
        cout << endl <<"---ERROR: Ya existe el disco----"<<endl;
        return;
    }
    int tam;
    if(this->u == "k"){ //kb
        tam =  this->size * 1024;
    }else if(this->u == "m"){ //mb
        tam =  this->size * 1024 * 1024;
    }else{
        cout << endl << "---UNIDADES DE SIZE INCORRECTAS---" << endl;
    }
    archivo =fopen(this->path.c_str(),"wb");
    fwrite("\0",1,1,archivo);
    fseek(archivo,tam,SEEK_SET);
    fwrite("\0",1,1,archivo);
    ine.mbr_tamano = tam;
    //Fecha y hora
    time_t t;
    struct tm *tm;
    char fechayhora[20];
    t = time(NULL);
    tm = localtime(&t);
    strftime(fechayhora, 20, "%d/%m/%Y %H:%M:%S", tm);
    strcpy(ine.mbr_fecha_creacion, fechayhora); // dato del mbr
    ine.mbr_disk_signature = rand() % 100; // dato para la etiqueta del disco
    // verificamos el FIT
    if(this->f.empty()){
        ine.disk_fit = 'f';
    }else if(this->f[0] == 'b'){
        ine.disk_fit = 'b';
    }else if(this->f[0] == 'f'){
        ine.disk_fit = 'f';
    }else if (this->f[0] == 'w'){
        ine.disk_fit = 'w';
    }else{
        cout << endl << "----ERROR EN LA CONFIGURACION DEL DISCO---" << endl;
    }
    // CREAMOS LAS 4 PARTICIONES
    particion vacia;
    vacia.part_status= '0';//status inactivo
    vacia.part_type= '-';
    vacia.part_fit= '-';
    vacia.part_start= -1;
    vacia.part_size= -1;
    vacia.part_name[0] = '\0';
    //lleno el disco de las 4 particiones
    for(int i = 0 ; i < 4 ; i ++){
        ine.mbr_partitions[i] = vacia;
    }
    //cout << "Nuevo disco: \nSize: " << ine.mbr_tamano << "\ndate: " << ine.mbr_fecha_creacion << "\nFit: " << ine.disk_fit << "\nDisk_Signature: " << ine.mbr_disk_signature << endl;
    //cout << "Bits del MBR: " << sizeof(mbr) << endl;

    //MBR al disco
    //archivo = fopen(this->path.c_str(), "rb+"); //modo de escritura mixto permite actualizar un fichero sin borrar el contenido anterior
    fseek(archivo,0,SEEK_SET);
    fwrite(&ine, sizeof(mbr), 1, archivo);
    fclose(archivo);
    printf(" -- DISCO CREADO CORRECTAMENTE --  \nSE AGREGO EL MBR DE MANERA CORRECTA\n");
}

string mkdisk::ToLower(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}

void mkdisk::CrearRuta() {
    string item;
    stringstream ruta;
    stringstream ss (path);
    int x = 0;
    while (getline(ss, item, '/')) {
        if(x != 0){
            if (!punto(item)){
                ruta << "/" << item;
            }
        }
        x++;
    }
    //cout << ruta.str() << endl;
    if (filesystem::create_directories(ruta.str())){
        cout << endl <<"-------SE CREO LA RUTA YA QUE NO EXISTIA---------" << endl;
    }
}

bool mkdisk::punto(string cadena) {
    for (int i = 0; i < cadena.length(); i++){
        if (cadena[i] == '.')
            return true;
    }
    return false;
}


