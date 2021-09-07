//
// Created by daniel on 11/08/21.
//

#include "rmdisk.h"

rmdisk::rmdisk(string parametro) {
    bool encontrado = false;
    for (int i = 0; i < parametro.length(); ++i) {
        if (parametro[i] == '='){
            encontrado = true;
            break;
        }
    }
    if (!encontrado){
        cout << "-- ERROR PARAMETRO NO ASIGNADO --" << endl;
        return;
    }
    stringstream input(parametro);
    string name, info;
    getline(input, name, '=');
    getline(input, info, '=');
    if (name == "-path"){
        this->path = info;
    }else{
        cout << " PARAMETRO NO ENCONTRADO EN EL COMANDO RMDISK" << endl;
        return;
    }
    this->borrarDisco();
}

void rmdisk::borrarDisco() {
    string respuesta;
    cout << endl << "......................................................" << endl;
    while(!(respuesta == "s" || respuesta == "n")){
        cout << "Seguro que deseas eliminar este disco? [s/n]" << endl;
        cin >> respuesta;
    }
    cout << "......................................................" << endl << endl;
    if (respuesta == "n"){
        cout << "----- ACCION DEL COMANDO CANCELADO -----" << endl << endl;
        return;
    }
    if (remove(path.c_str()) == 0)
    {
        cout << endl <<"--- DISCO ELIMINADO EXITOSAMENTE ---" << endl;
    }
    else
    {
        cout << endl <<"-- DISCO NO ENCONTRADO -- " << endl;
    }
}
