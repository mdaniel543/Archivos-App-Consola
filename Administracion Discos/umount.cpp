//
// Created by daniel on 19/08/21.
//

#include "umount.h"

umount::umount(vector<string> parametros) {
    for (string param : parametros){
        stringstream input_stringstream(param);
        string name, info;
        getline(input_stringstream, name, '=');
        getline(input_stringstream, info, '=');
        name = ToLower(name);
        info = ToLower(info);
        if (name == "-id"){
            this->id = info;
        }
    }
    if (id.empty()){
        cout << endl << "-- FALTA PARAMETROS PARA EL COMANDO UMOUNT --" << endl;
        return;
    }
}

void umount::desmontarParticion() {
    bool sia = false;
    int i;
    for (i = 0; i < 10; ++i) {
        if (montadas.discos[i].estado == 1){
            for (int j = 0; j < 24; ++j) {
                if (montadas.discos[i].particiones[j].estado == 1){
                    if (montadas.discos[i].particiones[j].id == this->id){
                        montadas.discos[i].particiones[j].estado = 0;
                        sia = true;
                        cout << "-- DESMONTANDO PARTICION: " << montadas.discos[i].particiones[j].nombre << " CON ID: " << montadas.discos[i].particiones[j].id << endl << endl;
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
    bool sihay = false;
    for (int k = 0; k < 24; ++k) {
        if (montadas.discos[i].particiones[k].estado == 1){
            sihay = true;
            break;
        }
    }
    if(!sihay){
        montadas.discos[i].estado = 0;
    }
    this->montadas.imprimirMontajes();
}

string umount::ToLower(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}
