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
#include "mount.h"
#include <cstring>
#include <filesystem>

using namespace std;

class fdisk {
public:
    fdisk(vector<string> parametros, mount montadas);
    void CrearParticion();
    void printDisco(FILE* arch);
    void validarDisco(particion part);
    void particionPrimExt(FILE *file, particion particionu, mbr Disk);
    void particionLogica(FILE *file, particion particionu, mbr Disk, particion Extendida);
    int fitex(char fit);
    void modificarParticion();
    void deleteParticion();
    string ToLower(string cadena);
    mount montadas;
private :
    int size = 0;
    string u;
    string path;
    string type;
    string fit;
    string del;
    string name;
    int add = 0;
};


#endif //MIA_PROYECTO1_201709450_FDISK_H
