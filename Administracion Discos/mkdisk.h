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

using namespace std;

class mkdisk {
public:
    mkdisk(vector<string> parametros);
    void crearDisco();

    const string &getSize() const;
    void setSize(const string &size);

    const string &getF() const;
    void setF(const string &f);

    const string &getU() const;
    void setU(const string &u);

    const string &getPath() const;
    void setPath(const string &path);

private:
    string size;
    string f;
    string u;
    const char *path;

};


#endif //MIA_PROYECTO1_201709450_MKDISK_H
