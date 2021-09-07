//
// Created by daniel on 12/08/21.
//

#include "exec.h"

exec::exec(string parametro) {
    bool encontrado = false;
    for (int i = 0; i < parametro.length(); ++i) {
        if (parametro[i] == '='){
            encontrado = true;
            break;
        }
    }
    if (!encontrado){
        cout << "-- ERROR PARAMETRO NO ASIGNADO --" << endl;
        return;
    }
    stringstream input(parametro);
    string name, info;
    getline(input, name, '=');
    getline(input, info, '=');
    if (name == "-path"){
        this->path = info;
    }else{
        cout << " PARAMAETRO NO ENCONTRADO EN EL COMANDO EXEC" << endl;
        return;
    }
   this->leerArchivo();
}

void exec::leerArchivo() {
    ifstream infile(path.c_str());
    string comando;
    string comandoCompleto;
    Interprete* inter = new Interprete(true);
    char esc_char = 27;
    while (getline(infile, comando))
    {
        string separadoCom = "";
        if (comando.empty() == false && comando.length() > 2){
            cout << endl << esc_char << "[3m" << comando << esc_char << "[0m" << endl;
            for (int i = 0; i < comando.length(); ++i) {
                if(comando[i] != '#') {
                    separadoCom += comando[i];
                }else{
                    break;
                }
            }
            if(comando.at(0) != '#'){
                cout << endl;
                inter->SepararComando(separadoCom);
            }
        }
    }
}

