//
// Created by daniel on 19/08/21.
//

#ifndef MIA_PROYECTO1_201709450_UMOUNT_H
#define MIA_PROYECTO1_201709450_UMOUNT_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "../Discos/estructuras.h"
#include "mount.h"
#include <cstring>
#include <filesystem>


class umount {
public:
    umount(vector<string> parametros, mount montadas);
    void desmontarParticion();
    mount montadas;
    string ToLower(string cadena);
private:
    string id;
};


#endif //MIA_PROYECTO1_201709450_UMOUNT_H
