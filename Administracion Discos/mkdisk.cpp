//
// Created by daniel on 5/08/21.
//


#include "mkdisk.h"

mkdisk::mkdisk(vector<string> parametros) {
    for (string param : parametros){
        stringstream input_stringstream(param);
        string name, info;

        getline(input_stringstream, name, '=');
        getline(input_stringstream, info, '=');
        name = ToLower(name);
        cout << name << endl;
        cout << info << endl;
        cout << "----------" << endl;

        if (name == "-path"){
            path = info;
        }
        info = ToLower(info);
        if(name == "–u"){
            u = info;
        }else if(name == "-size"){
            size = stoi(info);
        }else if(name == "-f"){
            f = info;
        }
    }
}

void mkdisk::crearDisco() {
    cout << "--------------Crear Disco-------------" << endl;
    mbr ine;
    FILE  *archivo = fopen(path.c_str(), "r");
    if(archivo != NULL){
        cout<<"Ya existe el disco"<<endl;
        return;//error
    }

    int tam;
    if(this->u == "k"){ //kb
        tam =  this->size * 1024;
    }
    else{ //mb
        tam =  this->size * 1024 * 1024;
    }
    cout << size << endl;
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
    strftime(fechayhora, 20, "%Y/%m/%d %H:%M:%S", tm);

    strcpy(ine.mbr_fecha_creacion, fechayhora); // dato del mbr
    cout << ine.mbr_fecha_creacion << endl;

    ine.mbr_disk_signature = (rand() % 100); // dato para la etiqueta del disco

    // verificamos el FIT
    if(this->f.empty()==true){
        strcpy( ine.disk_fit,"F");
    }else{
        strcpy(ine.disk_fit, this->f.c_str());
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
    for(int i = 0 ; i < 4 ; i ++)
        ine.mbr_partitions[i] = vacia;

    cout << "Nuevo disco: \nSize: " << ine.mbr_tamano << "\ndate: " << ine.mbr_fecha_creacion << "\nFit: " << ine.disk_fit << "\nDisk_Signature: " << ine.mbr_disk_signature << endl;
    cout << "Bits del MBR: " << sizeof(mbr) << endl;

    //MBR al disco
    archivo = fopen(this->path.c_str(), "rb+"); //modo de escritura mixto permite actualizar un fichero sin borrar el contenido anterior
    if(archivo != NULL){
        fseek(archivo,0,SEEK_SET);
        fwrite(&ine, sizeof(mbr), 1, archivo);
        fclose(archivo);
        printf("DISCO CREADO CORRECTAMENTE \nSE AGREGO EL MBR DE MANERA CORRECTA\n");
    }else{
        printf("Error!\n No se puede acceder al disco, MBR no creado\n");
    }
}

string mkdisk::ToLower(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}


