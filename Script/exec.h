//
// Created by daniel on 12/08/21.
//

#ifndef MIA_PROYECTO1_201709450_EXEC_H
#define MIA_PROYECTO1_201709450_EXEC_H

#include "../Analizador/Interprete.h"

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <fstream>

using namespace std;

class exec {
public:
    exec(string parametro);
    void leerArchivo();
private:
    string path;
};


#endif //MIA_PROYECTO1_201709450_EXEC_H
