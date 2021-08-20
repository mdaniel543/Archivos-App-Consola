//
// Created by daniel on 19/08/21.
//

#include "mkfs.h"

mkfs::mkfs(vector<string> parametros) {
    for (string param : parametros){
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
}

void mkfs::formatearParticion() {
    bool sia = false;
    int i;
    for (i = 0; i < 10; ++i) {
        if (montadas.discos[i].estado == 1){
            for (int j = 0; j < 24; ++j) {
                if (montadas.discos[i].particiones[j].estado == 1){
                    if (montadas.discos[i].particiones[j].id == this->id){
                        sia = true;
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


}

string mkfs::ToLower(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}


