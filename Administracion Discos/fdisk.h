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
#include <cstring>
#include <filesystem>

using namespace std;

class fdisk {
public:
    fdisk(vector<string> parametros);
    void CrearParticion();
    void printDisco(FILE* arch);
    void validarDisco(particion part);
    void particionPrimaria(FILE *file, particion particionu, mbr Disk);
    void particionExtendida(FILE *file, particion particionu, mbr Disk);
    void particionLogica(FILE *file, particion particionu, mbr Disk);
    int fitDisk(mbr Disk);
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
