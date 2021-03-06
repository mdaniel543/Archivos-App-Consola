//
// Created by daniel on 5/08/21.
//

#include "Interprete.h"

Interprete::Interprete(bool script) {
    this->script = script;
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
    for (string cmd : ver) {
        if (cx == 0){
            comando = cmd;
        }else{
            parametros.push_back(cmd);
        }
        cx++;
    }
    ReconocerComando(ToLower(comando), parametros);
}

void Interprete::ReconocerComando(string comando, vector<string> parametros ){
    if (comando == "exec"){
        if(!script){
            new exec(parametros[0]);
        }else{
            cout << "El comando EXEC no se puede utilizar en un script" << endl;
        }
    }else if (comando == "mkdisk"){
        new mkdisk(parametros);// creo el disco en su clase
    }else if(comando == "rmdisk"){
        new rmdisk(parametros[0]);
    }else if(comando == "fdisk"){
        new fdisk(parametros, montar);
    }else if(comando == "mount"){
        this->ParametrosMount(parametros);
    }else if(comando == "umount"){
        umount* nuevo = new umount(parametros, montar);
        montar = nuevo->montadas;
    }else if(comando == "mkfs"){
        mkfs* nuevo = new mkfs(parametros, montar);
        montar = nuevo->montadas;
    }else if(comando == "rep"){
        new rep(parametros, montar);
    }
    else if(comando == "pause"){
        if (script){
            new pause();
        }else{
            cout << "---El comando PAUSE solo funciona en el script-----" << endl;
        }
    }
    else{
        cout << "***********************************************" << endl;
        cout << " ERROR comando: " << comando << " no encontrado" << endl;
        cout << "************************************************" << endl << endl;
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
                c = false;
            }
            regreso.push_back(cadena);
        }
    }
    return regreso;
}
//QUMEADO LA LISTAS DE LAS MONTADAS
void Interprete::ParametrosMount(vector<string> parametros) {
    string path, nombre;
    for (string param : parametros){
        bool encontrado = false;
        for (int i = 0; i < param.length(); ++i) {
            if (param[i] == '='){
                encontrado = true;
                break;
            }
        }
        if (!encontrado){
            cout << "-- ERROR PARAMETRO NO ASIGNADO --" << endl;
            return;
        }
        stringstream input_stringstream(param);
        string name, info;
        getline(input_stringstream, name, '=');
        getline(input_stringstream, info, '=');
        name = ToLower(name);
        if (name == "-path"){
            path = info;
            continue;
        }
        info = ToLower(info);
        if(name == "-name"){
            nombre = info;
        }else{
            cout << " PARAMAETRO NO ENCONTRADO EN EL COMANDO MOUNT" << endl;
            return;
        }
    }
    if (path.empty() || nombre.empty()){
        cout << endl << "-- FALTA PARAMETROS PARA EL COMANDO MOUNT --" << endl;
        return;
    }
    this->montar.montarParticion(path, nombre);
}
