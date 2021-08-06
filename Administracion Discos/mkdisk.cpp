//
// Created by daniel on 5/08/21.
//

#include "mkdisk.h"

mkdisk::mkdisk(vector<string> parametros) {
    for (string param : parametros){
        stringstream input_stringstream(param);
        string name, info;

        getline(input_stringstream, name, '=');
        getline(input_stringstream, info, '=');

        cout << name << endl;
        cout << info << endl;
        cout << "----------" << endl;

        if (name == "-path"){
            path = info;
        }else if(name == "-u"){
            u = info;
        }else if(name == "-size"){
            size = info;
        }else if(name == "-f"){
            f = info;
        }
    }
}

void mkdisk::crearDisco() {
    FILE  *disco = fopen(path,"w+b");
    if (disco){
        cout << "Archivo creado";
    }else{
        cout << "No se pudo crear";
    }
}

const string &mkdisk::getSize() const {
    return size;
}

void mkdisk::setSize(const string &size) {
    mkdisk::size = size;
}

const string &mkdisk::getF() const {
    return f;
}

void mkdisk::setF(const string &f) {
    mkdisk::f = f;
}

const string &mkdisk::getU() const {
    return u;
}

void mkdisk::setU(const string &u) {
    mkdisk::u = u;
}

const string &mkdisk::getPath() const {
    return path;
}

void mkdisk::setPath(const string &path) {
    mkdisk::path = path;
}
