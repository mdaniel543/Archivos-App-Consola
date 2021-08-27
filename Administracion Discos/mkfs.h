//
// Created by daniel on 19/08/21.
//

#ifndef MIA_PROYECTO1_201709450_MKFS_H
#define MIA_PROYECTO1_201709450_MKFS_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "../Discos/estructuras.h"
#include "mount.h"
#include <cstring>
#include <filesystem>
#include <math.h>

class mkfs {
public:
    mkfs(vector<string> parametros);
    void particionMontada();
    void formatear(FILE *file, mbr temp, particion auxp, ebr aux, bool primaria);
    string ToLower(string cadena);
    mount montadas;
private:
    string id;
    string type;
    string fs;
};


#endif //MIA_PROYECTO1_201709450_MKFS_H
