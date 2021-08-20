//
// Created by daniel on 11/08/21.
//

#ifndef MIA_PROYECTO1_201709450_RMDISK_H
#define MIA_PROYECTO1_201709450_RMDISK_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
using namespace std;

class rmdisk {
public:
    rmdisk(string parametro);
    void borrarDisco();
private:
    string path;
};


#endif //MIA_PROYECTO1_201709450_RMDISK_H
