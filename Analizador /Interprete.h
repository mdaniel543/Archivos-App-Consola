//
// Created by daniel on 5/08/21.
//

#ifndef MIA_PROYECTO1_201709450_INTERPRETE_H
#define MIA_PROYECTO1_201709450_INTERPRETE_H

#include "../Administracion Discos/mkdisk.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

using namespace std;

class Interprete {

public:
    Interprete();
    void interpretar();
    void SepararComando(string lineacomando);
    void ReconocerComando(string comando, vector<string> parametros);

    string ToLower(string cadena);

};


#endif //MIA_PROYECTO1_201709450_INTERPRETE_H
