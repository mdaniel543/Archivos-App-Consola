//
// Created by daniel on 5/08/21.
//

#include "Interprete.h"

Interprete::Interprete(bool script) {
    this->script = script;
}

void Interprete::interpretar() {
    string comando;
    bool seguir;
    vector<string> listaComandos;
    cout << "---------------Ingrese comando-------------" << endl;
    /*do {
        getline(cin, comando);
        seguir = comando == "c" ;
        if (seguir){
            cout << endl << "--Ingreso de comandos terminado--" << endl;
            break;
        }
        listaComandos.push_back(comando);
    }while(!seguir);*/
    listaComandos.push_back("exec -path=/home/daniel/Escritorio/prueba.sh"); // QUEMADO

    for (string cmd : listaComandos) {
        cout << cmd << endl;
        if (cmd.at(0) != '#'){
            SepararComando(cmd);
        }
    }
    cout << "======================" << endl;
}

void Interprete::SepararComando(string lineacomando) {
    vector<string> cms;
    istringstream isstream(lineacomando);
    string palabra;
    while(isstream >> palabra){ // separo por espacios
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
    if (comando == "exec"){
        if(!script){
            cout << "hola exec" << endl;
            cout << "---------" << endl;
            exec* nuevo = new exec(parametros[0]);
            nuevo->leerArchivo();
        }else{
            cout << "El comando EXEC no se puede utilizar en un script" << endl;
        }
    }else if (comando == "mkdisk"){
        cout << "hola mkdisk" << endl;
        cout << "-----------" << endl;
        mkdisk* nuevo = new mkdisk(parametros);// creo el disco en su clase
        nuevo->crearDisco();
    }else if(comando == "rmdisk"){
        cout << "hola rmdisk" << endl;
        cout << "-----------" << endl;
        rmdisk* nuevo = new rmdisk(parametros[0]);
        nuevo->borrarDisco();
    }else if(comando == "fdisk"){
        cout << "hola fdisk" << endl;
        cout << "----------" << endl;
        fdisk* nuevo = new fdisk(parametros);
        nuevo->CrearParticion();
    }
    else if(comando == "pause"){
        if (script){
            pause* nuevo = new pause();
        }else{
            cout << "---El comando PAUSE solo funciona en el script-----" << endl;
        }
    }
    else{
        cout << "*****************************" << endl;
        cout << " ERROR comando: " << comando << " no encontrado" << endl;
        cout << "*****************************" << endl;
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


