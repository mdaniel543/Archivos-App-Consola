//
// Created by daniel on 12/08/21.
//

#ifndef MIA_PROYECTO1_201709450_MOUNT_H
#define MIA_PROYECTO1_201709450_MOUNT_H


#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "../Discos/estructuras.h"
#include <cstring>
#include <filesystem>

using namespace std;

typedef struct{
    int numero;
    int estado = 0;
    string nombre;
    string id;
}particionMontada;

typedef struct{
    int letra = 0;
    string path;
    int estado;
    particionMontada particiones[24];
}DiscoMontado;

class mount {
public:
    mount();
    void montarParticion(string path, string name);
    void imprimirMontajes();
    int carnet = 50;
    int numeroparticion = 0;
    DiscoMontado discos[10];
};


#endif //MIA_PROYECTO1_201709450_MOUNT_H
