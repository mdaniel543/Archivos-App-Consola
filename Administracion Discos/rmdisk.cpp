//
// Created by daniel on 11/08/21.
//

#include "rmdisk.h"

rmdisk::rmdisk(string parametro) {
    stringstream input(parametro);
    string name, info;
    getline(input, name, '=');
    getline(input, info, '=');
    this->path = info;
}

void rmdisk::borrarDisco() {
    if (remove(path.c_str()) == 0)
    {
        cout << "Disco eliminado exitosamente." << endl;
    }
    else
    {
        cout << "Disco no existente" << endl;
    }
}
