//
// Created by daniel on 21/08/21.
//

#ifndef MIA_PROYECTO1_201709450_REP_H
#define MIA_PROYECTO1_201709450_REP_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "../Discos/estructuras.h"
#include "../Administracion Discos/mount.h"
#include <cstring>
#include <filesystem>
#include <fstream>

class rep {
public:
    rep(vector<string> parametros, mount montadas);
    void repMBR();
    void repDisk();
    void crearGrafo(string graphviz);
    string ToLower(string cadena);
    string nom(string pa);
    void CrearRuta();
    bool punto(string cadena);
    mount montadas;

private:
    string name;
    string path;
    string id;
    string ruta;
    string root;
};


#endif //MIA_PROYECTO1_201709450_REP_H
