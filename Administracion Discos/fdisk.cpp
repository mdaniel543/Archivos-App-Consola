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
        cout << name << endl;
        cout << info << endl;
        cout << "----------" << endl;
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
    // ----------------- BUSCAR DISCO --------------
    FILE *file;
    int sizePart;
    file = fopen(this->path.c_str(), "rb+"); // Se abre con permisos mixtos
    if (file == NULL) {
        cout << endl <<"Error: El disco no existe" << endl;
    }
    fseek(file, 0, SEEK_SET); // Moverse 0 caracteres desde el inicio del archivo
    mbr tempDisk;
    // Se recupera lo que hay en el archivo en tempMbr, del tamaño del struct mbr
    fread(&tempDisk, sizeof(mbr), 1, file);
}

void fdisk::printDisco() {
    FILE*arch;
    arch=fopen(path.c_str(),"rb+");
    if(arch == NULL){
        cout << "-----------DISCO NO ENCONTRADO-----------" << endl;
    }
    mbr MBR;
    fseek(arch, 0, SEEK_SET); // Nos posicionamos en el primer byte del archivo
    fread(&MBR,sizeof(mbr),1,arch);
    fclose(arch);
    cout << "-----------------------------DATOS DEL DISCO--------------------------------" << endl;

    cout << "MBR SIGNATURE: "<< MBR.mbr_disk_signature << endl;
    cout << "MBR SIZE: "<< MBR.mbr_tamano << endl;
    cout << "FECHA: "<< MBR.mbr_fecha_creacion << endl;
    cout << "FIT: " << MBR.disk_fit << endl;

    for(int i=0;i<4;i++){
        //if(MBR.mbr_partitions[i].part_status=='1')  {
            cout << "PARTICION : "<< i << endl;
            cout << "PARTICION STATUS : "<< MBR.mbr_partitions[i].part_status << endl;
            cout << "PARTICION TYPE : "<< MBR.mbr_partitions[i].part_type << endl;
            cout << "PARTICION FIT : "<< MBR.mbr_partitions[i].part_fit << endl;
            cout << "PARTICION START : "<< MBR.mbr_partitions[i].part_start << endl;
            cout << "PARTICION SIZE : "<< MBR.mbr_partitions[i].part_size << endl;
            cout << "PARTICION NAME : "<< MBR.mbr_partitions[i].part_name << endl;
            cout << endl;
       // }
    }
}



string fdisk::ToLower(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}



