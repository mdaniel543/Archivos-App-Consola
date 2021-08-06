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
        cms.push_back(ToLower(palabra));
    }
    int cx =0;
    string comando;
    vector<string> parametros;
    for (string cmd : cms) {
        if (cx == 0){
            comando = cmd;
        }else{
            parametros.push_back(cmd);
        }
        cx++;
    }
    ReconocerComando(comando, parametros);
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

    }else{
        cout << " ERROR comando: " << comando << " no encontrado" << endl;
    }
}










string Interprete::ToLower(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}


