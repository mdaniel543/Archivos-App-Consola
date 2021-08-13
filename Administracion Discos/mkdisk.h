//
// Created by daniel on 5/08/21.
//

#ifndef MIA_PROYECTO1_201709450_MKDISK_H
#define MIA_PROYECTO1_201709450_MKDISK_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "../Discos/estructuras.h"
#include <cstring>
#include <filesystem>

using namespace std;

class mkdisk {
public:
    mkdisk(vector<string> parametros);
    void crearDisco();
    void CrearRuta();
    bool punto(string cadena);
    string ToLower(string cadena);

private:
    int size;
    string f;
    string u;
    string path;

};


#endif //MIA_PROYECTO1_201709450_MKDISK_H
