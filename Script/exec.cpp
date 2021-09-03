//
// Created by daniel on 12/08/21.
//

#include "exec.h"

exec::exec(string parametro) {
    stringstream input(parametro);
    string name, info;
    getline(input, name, '=');
    getline(input, info, '=');
    this->path = info;
}

void exec::leerArchivo() {
    ifstream infile(path.c_str());
    string comando;
    string comandoCompleto;
    Interprete* inter = new Interprete(true);
    char esc_char = 27;
    while (getline(infile, comando))
    {
        if (comando.empty() == false && comando.length() > 2){
            cout << endl << esc_char << "[3m" << comando << esc_char << "[0m" << endl;
            if(comando.at(0) != '#'){
                cout << endl;
                inter->SepararComando(comando);
            }
        }
    }
}

