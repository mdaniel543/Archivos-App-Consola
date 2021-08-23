//
// Created by daniel on 21/08/21.
//

#include "rep.h"

rep::rep(vector<string> parametros, mount montadas) {
    for (string param : parametros){
        stringstream input_stringstream(param);
        string nam, info;
        getline(input_stringstream, nam, '=');
        getline(input_stringstream, info, '=');
        nam = ToLower(nam);
        if (nam == "-path"){
            this->path = info;
        }
        info = ToLower(info);
        if(nam == "-id"){
            this->id = info;
        }else if(nam == "-name"){
            this->name = info;
        }else if(nam == "-ruta"){
            this->ruta = info;
        }else if(nam == "-root"){
            this->root = info;
        }
    }
    this->montadas = montadas;
    if (this->name.empty() || this->path.empty() || this->id.empty()){
        cout << endl <<"-- ERROR EN EL COMANDO REP, HACE FALTA PARAMAETROS --" << endl << endl;
    }else if (this->name == "mbr"){
        this->repMBR();
    }else if(this->name == "disk"){
        this->repDisk();
    }

}

void rep::repMBR() {
    bool sia = false;
    int i;
    for (i = 0; i < 10; ++i) {
        if (montadas.discos[i].estado == 1){
            for (int j = 0; j < 24; ++j) {
                if (montadas.discos[i].particiones[j].estado == 1){
                    if (montadas.discos[i].particiones[j].id == this->id){
                        sia = true;
                        break;
                    }
                }
            }
        }
        if (sia){
            break;
        }
    }
    if (!sia){
        cout << "-- NO SE ENCONTRO LA PARTICION MONTADA --" << endl;
        return;
    }
    FILE *file;
    file = fopen(montadas.discos[i].path.c_str(), "rb+"); // Se abre con permisos mixtos
    if (file == NULL) {
        cout << endl <<"----ERROR: EL DISCO NO EXISTE-----" << endl;
        return;
    }
    mbr tempDisk;
    fseek(file, 0, SEEK_SET);
    fread(&tempDisk, sizeof(mbr), 1, file); // Se recupera lo que hay en el archivo en tempDISK, del tamaño del struct mbr
    ebr aux;
    string nombre = nom(montadas.discos[i].path);
    if (nombre == "."){
        nombre = "Disco";
    }
    bool otra = false;
    stringstream graph;
    graph << "digraph {" << endl;
    graph << "node [shape=none fontname=Helvetica]" << endl;
    graph << "n4 [label = <" << endl;
    graph << "<table border=\"2\">" << endl;
    graph << R"(<tr><td colspan="2" bgcolor="firebrick3">)" << nombre << "</td></tr>" << endl;
    graph << "<tr><td>Tamaño</td><td>"<< tempDisk.mbr_tamano <<"</td></tr>" << endl;
    graph << "<tr><td>Fecha de Creacion</td><td>"<< tempDisk.mbr_fecha_creacion <<"</td></tr>" << endl;
    graph << "<tr><td>Signature</td><td>"<< tempDisk.mbr_disk_signature <<"</td></tr>" << endl;
    graph << "<tr><td>FIT</td><td>"<< tempDisk.disk_fit <<"</td></tr>" << endl;
    for (int j = 0; j < 4; ++j) {
        if (tempDisk.mbr_partitions[j].part_status == '0') continue;
        graph << R"(<tr><td colspan="2" bgcolor="dodgerblue2">)" << j + 1 << ". " << tempDisk.mbr_partitions[j].part_name << "</td></tr>" <<endl;
        graph << "<tr><td>Estado</td><td>"<< tempDisk.mbr_partitions[j].part_status <<"</td></tr>" << endl;
        graph << "<tr><td>Tipo</td><td>"<< tempDisk.mbr_partitions[j].part_type <<"</td></tr>" << endl;
        graph << "<tr><td>FIT</td><td>"<< tempDisk.mbr_partitions[j].part_fit <<"</td></tr>" << endl;
        graph << "<tr><td>Tamaño</td><td>"<< tempDisk.mbr_partitions[j].part_size <<"</td></tr>" << endl;
        graph << "<tr><td>Inicio</td><td>"<< tempDisk.mbr_partitions[j].part_start <<"</td></tr>" << endl;
        if (tempDisk.mbr_partitions[j].part_type == 'e'){
            fseek(file, tempDisk.mbr_partitions[j].part_start, SEEK_SET);
            fread(&aux, sizeof(ebr), 1, file);
            int s = 0;
            do{
                if (aux.part_status != '0'){
                    s++;
                    graph << R"(<tr><td colspan="2" bgcolor="paleturquoise">)" << s << ". " << aux.part_name << "</td></tr>" << endl;
                    graph << "<tr><td>Estado</td><td>"<< aux.part_status <<"</td></tr>" << endl;
                    graph << "<tr><td>FIT</td><td>"<< aux.part_fit <<"</td></tr>" << endl;
                    graph << "<tr><td>Inicio</td><td>"<< aux.part_start <<"</td></tr>" << endl;
                    graph << "<tr><td>Tamaño</td><td>"<< aux.part_size <<"</td></tr>" << endl;
                    graph << "<tr><td>Next</td><td>"<< aux.part_next <<"</td></tr>" << endl;
                }
                if (aux.part_next > 0){
                    fseek(file, aux.part_next, SEEK_SET);
                    fread(&aux, sizeof(ebr), 1, file);
                    otra = true;
                }else{
                    otra = false;
                }
            }while(otra);
        }
    }
    graph << "</table>" << endl << ">]" << endl << "}";
    /*cout << "---" <<endl;
    cout << graph.str() << endl;
    cout << "---" <<endl;*/
    this->crearGrafo(graph.str());
    graph.clear();
}

void rep::repDisk() {
    bool sia = false;
    int i;
    for (i = 0; i < 10; ++i) {
        if (montadas.discos[i].estado == 1){
            for (int j = 0; j < 24; ++j) {
                if (montadas.discos[i].particiones[j].estado == 1){
                    if (montadas.discos[i].particiones[j].id == this->id){
                        sia = true;
                        break;
                    }
                }
            }
        }
        if (sia){
            break;
        }
    }
    if (!sia){
        cout << "-- NO SE ENCONTRO LA PARTICION MONTADA --" << endl;
        return;
    }
    FILE *file;
    file = fopen(montadas.discos[i].path.c_str(), "rb+"); // Se abre con permisos mixtos
    if (file == NULL) {
        cout << endl <<"----ERROR: EL DISCO NO EXISTE-----" << endl;
        return;
    }
    mbr tempDisk;
    fseek(file, 0, SEEK_SET);
    fread(&tempDisk, sizeof(mbr), 1, file); // Se recupera lo que hay en el archivo en tempDISK, del tamaño del struct mbr
    ebr aux;
    string nombre = nom(montadas.discos[i].path);
    if (nombre == "."){
        nombre = "Disco";
    }
    bool otra = false;
    int acumulado = 0;
    stringstream graph;
    graph << "digraph structs{" << endl;
    graph << "bgcolor = \"white:lightblue\"" << endl;
    graph << "label = \""<< nombre << "\"" << endl;
    graph << "node [shape = record];" << endl;
    graph << "struct[label = \"{{MBR \"" << endl;
    for (int j = 0; j < 4; ++j) {
        if (tempDisk.mbr_partitions[j].part_status == '0'){
            if(j == 3){
                if (acumulado > 0){
                    graph << "+ \"| libre \"" << endl;
                    acumulado = tempDisk.mbr_tamano - acumulado;
                    double porcentaje = ((double)acumulado/(double)tempDisk.mbr_tamano)*100.00;
                    graph << "+ \"&#92;n" << porcentaje << "% \"" <<endl;
                }else{
                    graph << "+ \"| libre \"" << endl;
                    acumulado = tempDisk.mbr_tamano - (tempDisk.mbr_partitions[j - 1].part_start + tempDisk.mbr_partitions[j-1].part_size);
                    double porcentaje = ((double)acumulado/(double)tempDisk.mbr_tamano)*100.00;
                    graph << "+ \"&#92;n" << porcentaje << "% \"" <<endl;
                }
            }else{
                if (tempDisk.mbr_partitions[j+1].part_status == '1'){
                    graph << "+ \"| libre \"" << endl;
                    acumulado += tempDisk.mbr_partitions[j+1].part_start - tempDisk.mbr_partitions[j].part_start;
                    double porcentaje = ((double)acumulado/(double)tempDisk.mbr_tamano)*100.00;
                    graph << "+ \"&#92;n" << porcentaje << "% \"" <<endl;
                    acumulado = 0;
                }else{
                    if (tempDisk.mbr_partitions[j].part_start < 0 || tempDisk.mbr_partitions[j+1].part_start < 0){
                        graph << "+ \"| libre \"" << endl;
                        acumulado = tempDisk.mbr_tamano - (tempDisk.mbr_partitions[j-1].part_start + tempDisk.mbr_partitions[j-1].part_size);
                        double porcentaje = ((double)acumulado/(double)tempDisk.mbr_tamano)*100.00;
                        graph << "+ \"&#92;n" << porcentaje << "% \"" <<endl;
                        break;
                    }else{ // 0> 0>
                        acumulado += tempDisk.mbr_partitions[j+1].part_start - tempDisk.mbr_partitions[j].part_start;
                    }
                }
            }
            continue;
        }
        if (tempDisk.mbr_partitions[j].part_type == 'e'){
            graph << "+ \"| { Extendida \"" << endl;
            double porcentaje = ((double)tempDisk.mbr_partitions[j].part_size/(double)tempDisk.mbr_tamano)*100.00;
            graph << "+ \"&#92;n" << porcentaje << "% \"" <<endl;
            fseek(file, tempDisk.mbr_partitions[j].part_start, SEEK_SET);
            fread(&aux, sizeof(ebr), 1, file);
            if (aux.part_size < 0){
                graph << "+ \" | {Libre} \"" << endl;
                graph << "+ \"}\"" << endl;
                continue;
            }
            graph << "+ \" |{ \"" << endl;
            int s = 0;
            do{
                if (s != 0){
                    graph << "+ \"|\"" << endl;
                }
                if (aux.part_status != '0'){
                    graph << "+ \" EBR \"" << endl;
                    graph << "+ \" | Logica \"" << endl;
                    double porcentaje = ((double)aux.part_size/(double)tempDisk.mbr_tamano)*100.00;
                    graph << "+ \"&#92;n" << porcentaje << "% \"" <<endl;
                }else{
                    graph << "+ \" Libre \"" << endl;
                    double porcentaje = ((double)aux.part_size/(double)tempDisk.mbr_tamano)*100.00;
                    graph << "+ \"&#92;n" << porcentaje << "% \"" <<endl;
                }
                if (aux.part_next > 0){
                    fseek(file, aux.part_next, SEEK_SET);
                    fread(&aux, sizeof(ebr), 1, file);
                    otra = true;
                }else{
                    if(aux.part_start + aux.part_size != tempDisk.mbr_partitions[j].part_start + tempDisk.mbr_partitions[j].part_size){
                        graph << "+ \" | Libre \"" << endl;
                        int sizere = (tempDisk.mbr_partitions[j].part_start + tempDisk.mbr_partitions[j].part_size) - (aux.part_start + aux.part_size);
                        double porcentaje = ((double)sizere/(double)tempDisk.mbr_tamano)*100.00;
                        graph << "+ \"&#92;n" << porcentaje << "% \"" <<endl;
                    }
                    otra = false;
                }
                s++;
            }while(otra);
            graph << "+ \"}} \"" << endl;
        }else{
            graph << "+ \" | Primaria\"" << endl;
            double porcentaje = ((double)tempDisk.mbr_partitions[j].part_size/(double)tempDisk.mbr_tamano)*100.00;
            graph << "+ \"&#92;n" << porcentaje << "% \"" <<endl;
        }
    }
    graph << "+ \"}}\"" << endl;
    graph << "];" << endl << "}";
    /*cout << "---" <<endl;
    cout << graph.str() << endl;
    cout << "---" <<endl;*/
    this->crearGrafo(graph.str());
    graph.clear();
}


void rep::crearGrafo(string graphviz) {
    this->CrearRuta();
    string n = this->nom(this->path);
    cout << n << endl;
    FILE  *archivo = fopen(this->path.c_str(), "r");
    string ruta = this->path;
    string pathPng = ruta.substr(0, ruta.size() - 4);
    pathPng += ".png";
    ofstream outfile(pathPng);
    outfile << graphviz.c_str();
    outfile.close();
    string comando = "dot -Tpng " + pathPng + " -o " + pathPng;
    system(comando.c_str());
    string ds = "display " + pathPng;
    system(ds.c_str());
}

string rep::ToLower(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}

string rep::nom(string pa) {
    string item;
    stringstream ruta;
    stringstream ss (pa);
    int x = 0;
    while (getline(ss, item, '/')) {
        if(x != 0){
            for (int i = 0; i < item.length(); i++){
                if (item[i] == '.')
                    return item;
            }
        }
        x++;
    }
    return ".";
}

void rep::CrearRuta() {
    string item;
    stringstream ruta;
    stringstream ss (this->path);
    int x = 0;
    while (getline(ss, item, '/')) {
        if(x != 0){
            if (!punto(item)){
                ruta << "/" << item;
            }
        }
        x++;
    }
    if (filesystem::create_directories(ruta.str())){
        cout << endl <<"-------SE CREO LA RUTA YA QUE NO EXISTIA---------" << endl;
    }
}

bool rep::punto(string cadena) {
    for (int i = 0; i < cadena.length(); i++){
        if (cadena[i] == '.')
            return true;
    }
    return false;
}