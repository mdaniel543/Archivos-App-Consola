//
// Created by daniel on 5/08/21.
//

#ifndef MIA_PROYECTO1_201709450_INTERPRETE_H
#define MIA_PROYECTO1_201709450_INTERPRETE_H

#include "../Administracion Discos/mkdisk.h"
#include "../Administracion Discos/rmdisk.h"
#include "../Script/exec.h"
#include "../Administracion Carpetas y Permisos/pause.h"
#include "../Administracion Discos/fdisk.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

class Interprete {

public:
    Interprete(bool script);
    void SepararComando(string lineacomando);
    void ReconocerComando(string comando, vector<string> parametros);
    vector<string> ReconocerComilla(vector<string> ant);

    string ToLower(string cadena);

private :
    bool script;

};


#endif //MIA_PROYECTO1_201709450_INTERPRETE_H
