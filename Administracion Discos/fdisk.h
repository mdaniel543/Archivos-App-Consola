//
// Created by daniel on 12/08/21.
//

#ifndef MIA_PROYECTO1_201709450_FDISK_H
#define MIA_PROYECTO1_201709450_FDISK_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "../Discos/estructuras.h"

using namespace std;

class fdisk {
public:
    fdisk(vector<string> parametros);
    void CrearParticion();
    void printDisco();
    string ToLower(string cadena);
private :
    int size;
    string u;
    string path;
    string type;
    string fit;
    string del;
    string name;
    string add;
};


#endif //MIA_PROYECTO1_201709450_FDISK_H
