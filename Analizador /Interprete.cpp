//
// Created by daniel on 5/08/21.
//

#include "Interprete.h"

Interprete::Interprete() {

}

void Interprete::interpretar() {
    string comando;
    bool seguir;
    vector<string> listaComandos;
    cout << "---------------Ingrese comando-------------" << endl;
    do {
        getline(cin, comando);
        seguir = comando == "c" ;
        if (seguir){
            cout << endl << "--Ingreso de comandos terminado--" << endl;
            break;
        }
        listaComandos.push_back(comando);
    }while(!seguir);

    for (string cmd : listaComandos) {
        cout << cmd << endl;
        cout << "--------------" << endl;
        SepararComando(cmd);
    }
}

void Interprete::SepararComando(string lineacomando) {
    vector<string> cms;
    istringstream isstream(lineacomando);
    string palabra;
    while(isstream >> palabra){
        cms.push_back(palabra);
    }

    vector<string> ver = ReconocerComilla(cms);

    int cx =0;
    string comando;
    vector<string> parametros;
    cout << "~~~" << endl;
    for (string cmd : ver) {
        if (cx == 0){
            comando = cmd;
        }else{
            parametros.push_back(cmd);
        }
        cx++;
        cout << cmd << endl;
    }
    cout << "~~~" << endl;
    ReconocerComando(ToLower(comando), parametros);
}

void Interprete::ReconocerComando(string comando, vector<string> parametros ){
    if (comando == "exec")
    {
        cout << "hola exec" << endl;
        cout << "---------" << endl;
    }
    else if (comando == "mkdisk")
    {
        cout << "hola mkdisk" << endl;
        cout << "-----------" << endl;
        // creo el disco en su clase
        mkdisk* nuevo = new mkdisk(parametros);
        nuevo->crearDisco();

    }else{
        cout << " ERROR comando: " << comando << " no encontrado" << endl;
    }
}

string Interprete::ToLower(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}

vector<string> Interprete::ReconocerComilla(vector<string> ant) {
    vector<string> regreso;
    stringstream comilla;
    string aux;
    bool c = false;
    bool trae = false;
    for (string cadena : ant) {
        for (int i = 0; i < cadena.size() ; ++i) {
            if (cadena[i] == '\"'){
                c = true;
                if (trae){
                    comilla << cadena;
                    cadena = comilla.str();
                    comilla.clear();
                    trae = false;
                    break;
                }else{
                    trae = true;
                    break;
                }
            }
        }
        if(trae){
            if (cadena[cadena.size()-1] == '\"'){
                trae = false;
            }else{
                comilla << cadena << " ";
            }
        }
        if (!trae){
            if (c){
                cadena.replace(cadena.find("\""), 1, "");
                cadena.replace(cadena.find("\""), 1, "");
                cout << "%" << cadena << endl ;
                c = false;
            }
            regreso.push_back(cadena);
        }
    }
    return regreso;
}


