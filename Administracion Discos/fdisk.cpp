//
// Created by daniel on 12/08/21.
//

#include "fdisk.h"

fdisk::fdisk(vector<string> parametros, mount montadas) {
    bool ad = false, prim = false;
    for (string param : parametros){
        stringstream input_stringstream(param);
        string name, info;
        getline(input_stringstream, name, '=');
        getline(input_stringstream, info, '=');
        name = ToLower(name);
        if (name == "-path"){
            path = info;
        }
        info = ToLower(info);
        if(name == "-u"){
            u = info;
        }else if(name == "-size"){
            size = stoi(info);
        }else if(name == "-f"){
            fit = info;
        }else if(name == "-type"){
            type = info;
        }else if(name == "-delete"){
            this->montadas = montadas;
            del = info;
        }else if(name == "-name"){
            this->name = info;
        }else if(name == "-add"){
            add = stoi(info);
            ad = true;
            if (this->size != 0){
                prim = true;
            }
        }
    }
    if (!this->del.empty() && this->add > 0){
        cout << "-- PARAMETROS INCOMPATIBLES --" << endl;
    }
    if (!del.empty()){
        this->deleteParticion();
    }else if(ad){
        if (prim){
            this->add = this->size;
        }
        this->modificarParticion();
    }else{
        this->CrearParticion();
    }
}

void fdisk::CrearParticion() {
    particion part;
    int numsize;
    if(this->u == "k" || this->u.empty()){ //kb
        numsize =  this->size * 1024;
    }else if(this->u == "b"){
        numsize = this->size;
    }else if(this->u == "m"){ //mb
        numsize =  this->size * 1024 * 1024;
    }else{
        cout << endl << "---UNIDADES DE SIZE INCORRECTAS---" << endl;
        return;
    }
    cout << numsize << endl;
    if (size < 0 ){
        cout << endl << "---ERRROR DE SIZE, DEBE SER MAYOR A CERO---" << endl;
        return;
    }
    char f;
    if(this->fit.empty()){
        f = 'w';
    }else if(this->fit[0] == 'b'){
        f = 'b';
    }else if(this->fit[0] == 'f'){
        f = 'f';
    }else if (this->fit[0] == 'w'){
        f = 'w';
    }else{
        cout << endl << "---- ERROR EN LA CONFIGURACION DE LA PARTICION ---" << endl;
        return;
    }
    char t;
    if(this->type.empty()){
        t = 'p';
    }else if(this->type[0] == 'e'){
        t = 'e';
    }else if(this->type[0] == 'p'){
        t = 'p';
    }else if (this->type[0] == 'l'){
        t = 'l';
    }else{
        cout << endl << "---- ERROR EN EL TIPO DE PARTICION ---" << endl;
        return;
    }
    if(this->name.empty()){
        cout << endl << "--- LA PARTICION TIENE QUE TENER NOMBRE --" << endl;
        return;
    }
    //CREO LA PARTICION
    part.part_status = '1';
    part.part_fit = f;
    strcpy(part.part_name, name.c_str());
    part.part_start = 0;
    part.part_size = numsize;
    part.part_type = t;
    //mando particion al disco
    this->validarDisco(part);
}
void fdisk::validarDisco(particion part) {
    //----------------- BUSCAR DISCO --------------
    FILE *file;
    file = fopen(this->path.c_str(), "rb+"); // Se abre con permisos mixtos
    if (file == NULL) {
        cout << endl <<"----ERROR: EL DISCO NO EXISTE-----" << endl;
        return;
    }
    mbr tempDisk;
    fseek(file, 0, SEEK_SET);
    fread(&tempDisk, sizeof(mbr), 1, file); // Se recupera lo que hay en el archivo en tempDISK, del tamaño del struct mbr
    //fclose(file);
    // VALIDACIONES
    int disco4Particiones = 0;
    int disponible = 0;
    bool existeExtendida = false;
    particion auxE;
    for (int i = 0; i < 4; ++i) {
        if (tempDisk.mbr_partitions[i].part_status != '0'){
            disco4Particiones++;
            if (tempDisk.mbr_partitions[i].part_type == 'e'){
                existeExtendida = true;
                auxE = tempDisk.mbr_partitions[i];
            }
            disponible += tempDisk.mbr_partitions[i].part_size;
        }else{
            if (part.part_type != 'l'){
                if(i != 3){
                    if (i == 0){
                        if (tempDisk.mbr_partitions[i].part_start > 0 && tempDisk.mbr_partitions[i + 1].part_start > 0 ){
                            int sizeRecuperado = tempDisk.mbr_partitions[i + 1].part_start - tempDisk.mbr_partitions[i].part_start;
                            if (part.part_size > sizeRecuperado){
                                disponible += sizeRecuperado;
                            }
                        }
                    }else{
                        if (tempDisk.mbr_partitions[i].part_start > 0 && tempDisk.mbr_partitions[i + 1].part_start > 0 ){
                            int sizeRecuperado = tempDisk.mbr_partitions[i + 1].part_start - tempDisk.mbr_partitions[i].part_start;
                            if (part.part_size > sizeRecuperado){
                                disponible += sizeRecuperado;
                            }
                        }
                    }
                }else{
                    if (tempDisk.mbr_partitions[i - 1].part_status == '1'){
                        int sizeRecuperado = tempDisk.mbr_tamano - (tempDisk.mbr_partitions[i -1].part_start + tempDisk.mbr_partitions[i -1].part_size);
                        if (part.part_size > sizeRecuperado){
                            disponible += sizeRecuperado;
                        }
                    }
                }
            }
        }
    }
    disponible = tempDisk.mbr_tamano - disponible;
    if (disco4Particiones >= 4 && part.part_type != 'l'){
        cout << endl << "-- EXISTEN YA EL MAXIMO DE PARTICIONES EN EL DISCO --";
        return;
    }
    if(tempDisk.mbr_tamano < part.part_size && part.part_type != 'l'){
        cout << endl << "-- PARTICION MAS GRANDE QUE EL DISCO -- " << endl;
        return;
    }
    if (disponible < part.part_size && part.part_type != 'l'){
        cout << endl << "-- EL DISCO NO TIENE ESPACIO DISPONIBLE PARA ESTA PARTICION --" << endl;
        return;
    }
    if (existeExtendida){
        if (part.part_type == 'e'){
            cout << endl << "-- YA EXISTE UNA PARTICION EXTENDIDA EN EL DISCO --" << endl;
            return;
        }else if (part.part_type == 'l'){
            ebr aux;
            fseek(file, auxE.part_start, SEEK_SET);
            fread(&aux, sizeof(ebr), 1, file);
            int contador = 0;
            if (part.part_size > auxE.part_size){
                cout << endl << "--- LA PARTICION LOGICA ES MAS GRANDE QUE LA EXTENDIDA ---" << endl;
                return;
            }
            while (aux.part_next != -1){
                if (aux.part_status != '0'){
                    contador += aux.part_size;
                }else{
                    if (aux.part_size != -1){
                        if (part.part_size > aux.part_size){
                            contador += aux.part_size;
                        }
                    }
                }
                fseek(file, aux.part_next, SEEK_SET);
                fread(&aux, sizeof(ebr), 1, file);
            }
            if (aux.part_size != -1){
                if (aux.part_status != '0'){
                    contador += aux.part_size;
                }
                contador = auxE.part_size - contador;
                if (part.part_size > contador){
                    cout << endl << "--- YA NO HAY ESPACIO EN LA EXTENDIDA PARA ESTA PARTICION LOGICA ---" << endl;
                    return;
                }
            }
        }
    }else{
        if (part.part_type == 'l'){
            cout << endl << "-- NO EXISTE UNA PARTICION EXTENDIDA EN EL DISCO PARA UNA LOGICA -- " << endl;
            return;
        }
    }
    /// MANDO A COLOCAR PARTICION
    if (part.part_type == 'l'){
        particionLogica(file, part, tempDisk, auxE);
    }else{
        particionPrimExt(file, part, tempDisk);
    }
    this->printDisco(file);/// MANDO A VERIFICAR SI SE GUARDARON CAMBIOS EN EL DISCO
}

void fdisk::particionPrimExt(FILE *file, particion particionu, mbr Disk) {
    bool sifunciono = false;
    int j = -1;
    int auxu = -1;
    switch (fitex(Disk.disk_fit)) {
        case 1: // BEST FIT
            for (int i = 0; i < 4; ++i) {
                if (Disk.mbr_partitions[i].part_status == '0'){
                    if (i == 0){
                        if (Disk.mbr_partitions[i + 1].part_start != -1){
                            int sizeRecuperado =  Disk.mbr_partitions[i + 1].part_start - Disk.mbr_partitions[i].part_start;
                            if (particionu.part_size > sizeRecuperado){
                                continue;
                            }
                            auxu = Disk.mbr_partitions[i + 1].part_start - Disk.mbr_partitions[i].part_start;
                            j = i;
                        }else{
                            j = i;
                            break;
                        }
                    }else{
                        if(i != 3){
                            if (Disk.mbr_partitions[i + 1].part_start != -1){
                                int sizeRecuperado =  Disk.mbr_partitions[i + 1].part_start - Disk.mbr_partitions[i].part_start;
                                if (particionu.part_size > sizeRecuperado){
                                    continue;
                                }
                                if (auxu != -1){
                                    if (auxu > sizeRecuperado){
                                        auxu = sizeRecuperado;
                                        j =  i;
                                    }
                                }else{
                                    auxu = sizeRecuperado;
                                    j = i;
                                }
                            }else{
                                j = i;
                                break;
                            }
                        }else{
                            if (Disk.mbr_partitions[i].part_start != -1){
                                int sizeRecuperado = Disk.mbr_tamano - Disk.mbr_partitions[i].part_start;
                                if (auxu != -1){
                                    if (auxu > sizeRecuperado){
                                        j = i;
                                    }
                                    break;
                                }
                            }
                            j = i;
                        }
                    }
                }
            }
            sifunciono = true;
            if (j == 0){
                particionu.part_start = sizeof(mbr);
            }else{
                if (Disk.mbr_partitions[j].part_start != -1){
                    particionu.part_start = Disk.mbr_partitions[j].part_start;
                }else{
                    particionu.part_start = Disk.mbr_partitions[j - 1].part_start + Disk.mbr_partitions[j - 1].part_size;
                }
            }
            Disk.mbr_partitions[j] = particionu;
            break;
        case 2: // FIRST FIT
            for (int i = 0; i < 4; ++i) {
                if (Disk.mbr_partitions[i].part_status == '0'){
                    if (i == 0){
                        if (Disk.mbr_partitions[i + 1].part_start != -1){
                            int sizeRecuperado =  Disk.mbr_partitions[i + 1].part_start - Disk.mbr_partitions[i].part_start;
                            if (particionu.part_size > sizeRecuperado){
                                continue;
                            }
                        }
                        particionu.part_start = sizeof(mbr);
                        Disk.mbr_partitions[i] = particionu;
                        sifunciono = true;
                        break;
                    }else{
                        if(i != 3){
                            if (Disk.mbr_partitions[i + 1].part_start != -1){
                                int sizeRecuperado =  Disk.mbr_partitions[i + 1].part_start - Disk.mbr_partitions[i].part_start;
                                if (particionu.part_size > sizeRecuperado){
                                    continue;
                                }
                            }
                        }
                        if (Disk.mbr_partitions[i].part_start != -1){
                            particionu.part_start = Disk.mbr_partitions[i].part_start;
                        }else{
                            particionu.part_start = Disk.mbr_partitions[i - 1].part_start + Disk.mbr_partitions[i - 1].part_size;
                        }
                        Disk.mbr_partitions[i] = particionu;
                        sifunciono = true;
                        break;
                    }
                }
            }
            break;
        case 3: //  WORST FIT
            for (int i = 0; i < 4; ++i) {
                if (Disk.mbr_partitions[i].part_status == '0'){
                    if (i == 0){
                        if (Disk.mbr_partitions[i + 1].part_start != -1){
                            int sizeRecuperado =  Disk.mbr_partitions[i + 1].part_start - Disk.mbr_partitions[i].part_start;
                            if (particionu.part_size > sizeRecuperado){
                                continue;
                            }
                            auxu = Disk.mbr_partitions[i + 1].part_start - Disk.mbr_partitions[i].part_start;
                            j = i;
                        }else{
                            j = i;
                            break;
                        }
                    }else{
                        if(i != 3){
                            if (Disk.mbr_partitions[i + 1].part_start != -1){
                                int sizeRecuperado =  Disk.mbr_partitions[i + 1].part_start - Disk.mbr_partitions[i].part_start;
                                if (particionu.part_size > sizeRecuperado){
                                    continue;
                                }
                                if (auxu != -1){
                                    if (auxu < sizeRecuperado){
                                        auxu = sizeRecuperado;
                                        j =  i;
                                    }
                                }else{
                                    auxu = sizeRecuperado;
                                    j = i;
                                }
                            }else{
                                j = i;
                                break;
                            }
                        }else{
                            if (Disk.mbr_partitions[i].part_start != -1){
                                int sizeRecuperado = Disk.mbr_tamano - Disk.mbr_partitions[i].part_start;
                                if (auxu != -1){
                                    if (auxu < sizeRecuperado){
                                        j = i;
                                    }
                                    break;
                                }
                            }
                            j = i;
                        }
                    }
                }
            }
            sifunciono = true;
            if (j == 0){
                particionu.part_start = sizeof(mbr);
            }else{
                if (Disk.mbr_partitions[j].part_start != -1){
                    particionu.part_start = Disk.mbr_partitions[j].part_start;
                }else{
                    particionu.part_start = Disk.mbr_partitions[j - 1].part_start + Disk.mbr_partitions[j - 1].part_size;
                }
            }
            Disk.mbr_partitions[j] = particionu;
            break;
        default:
            break;
    }
    if (!sifunciono){
        cout << "-- NO SE HA PODIOD CREAR LA PARTICION --" << endl;
        return;
    }
    fseek(file, 0, SEEK_SET);
    fwrite(&Disk, sizeof(mbr), 1, file);
    if (particionu.part_type == 'e'){//INICIO UN EBR VACIO
        ebr particionE;
        particionE.part_status = '0';
        particionE.part_fit = particionu.part_fit;
        particionE.part_size = -1; //indicador
        particionE.part_name[0] = '\0';
        particionE.part_next = -1;
        particionE.part_start = particionu.part_start + sizeof(ebr);
        fseek(file, particionu.part_start, SEEK_SET);
        fwrite(&particionE, sizeof(ebr), 1, file); /// coloco al inicio de la extendida recien creada
    }
}

void fdisk::particionLogica(FILE *file, particion particionu, mbr Disk, particion Extendida) {
    ebr aux;
    ebr particionE;
    particionE.part_status = particionu.part_status;
    particionE.part_fit = particionu.part_fit;
    particionE.part_size = particionu.part_size;
    strcpy(particionE.part_name, particionu.part_name);
    fseek(file, Extendida.part_start, SEEK_SET); // ME ME COLOCO EN LA EXTENDIDA
    fread(&aux, sizeof(ebr), 1, file); /// LEO
    int mi = -1;
    particionE.part_next = -1;
    switch (fitex(Extendida.part_fit)) {
        case 1: // BEST FIT
            while(aux.part_next != -1){
                if (aux.part_status == '0'){
                    if (aux.part_size > particionE.part_size){
                        if (mi != -1){
                            if (mi < aux.part_size){
                                fseek(file, aux.part_next, SEEK_SET);
                                fread(&aux, sizeof(ebr), 1, file);
                                continue;
                            }
                        }
                        mi = aux.part_size;
                        particionE.part_next = aux.part_next;
                        particionE.part_start = aux.part_start;
                    }
                }
                fseek(file, aux.part_next, SEEK_SET);
                fread(&aux, sizeof(ebr), 1, file);
            }
            if (aux.part_size == -1 || aux.part_status == '0' || mi != -1){
                if (aux.part_size ==-1)
                    particionE.part_start = aux.part_start;
                fseek(file, particionE.part_start - sizeof(ebr), SEEK_SET);
                fwrite(&particionE, sizeof(ebr), 1, file);
            }else{
                particionE.part_start = aux.part_start + aux.part_size + sizeof(ebr);
                aux.part_next = aux.part_start + aux.part_size;
                fseek(file, aux.part_next, SEEK_SET);//creo el nuevo ebr
                fwrite(&particionE, sizeof(ebr), 1, file);
                fseek(file, aux.part_start - sizeof(ebr), SEEK_SET);//actuzalizo
                fwrite(&aux, sizeof(ebr), 1, file);
            }
            break;
        case 2: // FIRST FIT
            while(aux.part_next != -1){
                if (aux.part_status == '0'){
                    if (aux.part_size > particionE.part_size){
                        particionE.part_next = aux.part_next;
                        break;
                    }
                }
                fseek(file, aux.part_next, SEEK_SET);
                fread(&aux, sizeof(ebr), 1, file);
            }
            if (aux.part_size == -1 || aux.part_status == '0'){
                particionE.part_start = aux.part_start;
                fseek(file, aux.part_start - sizeof(ebr), SEEK_SET);
                fwrite(&particionE, sizeof(ebr), 1, file);
            }else{
                particionE.part_start = aux.part_start + aux.part_size + sizeof(ebr);
                aux.part_next = aux.part_start + aux.part_size;
                fseek(file, aux.part_next, SEEK_SET);//creo el nuevo ebr
                fwrite(&particionE, sizeof(ebr), 1, file);
                fseek(file, aux.part_start - sizeof(ebr), SEEK_SET);//actuzalizo
                fwrite(&aux, sizeof(ebr), 1, file);
            }
            break;
        case 3: // WORST FIT
            while(aux.part_next != -1){
                if (aux.part_status == '0'){
                    if (aux.part_size > particionE.part_size){
                        if (mi != -1){
                            if (mi > aux.part_size){
                                fseek(file, aux.part_next, SEEK_SET);
                                fread(&aux, sizeof(ebr), 1, file);
                                continue;
                            }
                        }
                        mi = aux.part_size;
                        particionE.part_next = aux.part_next;
                        particionE.part_start = aux.part_start;
                    }
                }
                fseek(file, aux.part_next, SEEK_SET);
                fread(&aux, sizeof(ebr), 1, file);
            }
            if (aux.part_size == -1 || aux.part_status == '0' || mi != -1){
                if (aux.part_size ==-1)
                    particionE.part_start = aux.part_start;
                fseek(file, particionE.part_start - sizeof(ebr), SEEK_SET);
                fwrite(&particionE, sizeof(ebr), 1, file);
            }else{
                particionE.part_start = aux.part_start + aux.part_size + sizeof(ebr);
                aux.part_next = aux.part_start + aux.part_size;
                fseek(file, aux.part_next, SEEK_SET);//creo el nuevo ebr
                fwrite(&particionE, sizeof(ebr), 1, file);
                fseek(file, aux.part_start - sizeof(ebr), SEEK_SET);//actuzalizo
                fwrite(&aux, sizeof(ebr), 1, file);
            }
            break;
        default:
            break;
    }
}

void fdisk::deleteParticion() {
    int i;
    for (i = 0; i < 10; ++i) {
        if (montadas.discos[i].estado == 1){
            for (int j = 0; j < 24; ++j) {
                if (montadas.discos[i].particiones[j].estado == 1){
                    if (montadas.discos[i].particiones[j].nombre == this->name){
                        cout << endl << "-- LA PARTICION NO SE PUEDE ELIMINAR YA QUE ESTA MONTADA ---" << endl << endl;
                        return;
                    }
                }
            }
        }
    }
    FILE *file;
    file = fopen(this->path.c_str(), "rb+"); // Se abre con permisos mixtos
    if (file == NULL) {
        cout << endl <<"----ERROR: EL DISCO NO EXISTE-----" << endl;
        return;
    }
    mbr MBR;
    int pos;
    fseek(file, 0, SEEK_SET);
    fread(&MBR, sizeof(mbr), 1, file); // Se recupera lo que hay en el archivo en MBR, del tamaño del struct mbr
    ebr aux;
    particion auxp;
    bool otra;
    bool pe = false;
    bool l = false;
    string nombre = "";
    for (int i = 0; i < 4; ++i) {
        if (MBR.mbr_partitions[i].part_name == this->name){
            pe = true;
            pos = i;
            auxp = MBR.mbr_partitions[i];
            nombre = auxp.part_name;
            break;
        }
        if (MBR.mbr_partitions[i].part_type == 'e'){
            fseek(file, MBR.mbr_partitions[i].part_start, SEEK_SET);
            fread(&aux, sizeof(ebr), 1, file);
            do{
                if(aux.part_name == this->name){
                    l = true;
                    auxp = MBR.mbr_partitions[i];
                    nombre = aux.part_name;
                    break;
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
        if (l){
            break;
        }
    }
    if (!pe && !l){
        cout << endl << "-- NO EXISTE LA PARTICION EN EL DISCO --" << endl << endl;
        return;
    }
    string respuesta;
    cout << endl << "......................................................" << endl;
    while(!(respuesta == "s" || respuesta == "n")){
        cout << "Seguro que deseas eliminar la particion \"" << nombre <<"\"? [s/n]" << endl;
        cin >> respuesta;
    }
    cout << "......................................................" << endl << endl;
    if (respuesta == "n"){
        cout << "----- ACCION DEL COMANDO CANCELADO -----" << endl << endl;
        return;
    }
    if (l){ // si la particion es logica
        if (this->del == "fast" || this->del == "full"){
            cout << "Particion logica eliminada: " << aux.part_name <<  endl;
            aux.part_status = '0'; // LISTO :) PARTICION ELIMINADA
            fseek(file, aux.part_start - sizeof(ebr), SEEK_SET);
            fwrite(&aux, sizeof(ebr), 1, file);
        }else{
            cout << "--VALOR DELETE INCORRECTO--" << endl << endl;
            return;
        }

    }
    if(pe){ // si la particion es primario o extendida
        if (this->del == "fast"){
            auxp.part_status = '0';
            MBR.mbr_partitions[pos] = auxp;
            fseek(file, 0, SEEK_SET);
            fwrite(&MBR, sizeof(mbr), 1, file);
        }else if(this->del == "full"){
            auxp.part_status = '0';
            auxp.part_type = '\0';
            auxp.part_fit = '\0';
            //auxp.part_start = '\0';
            auxp.part_size = '\0';
            strcpy(auxp.part_name, "");
            MBR.mbr_partitions[pos] = auxp;
            fseek(file, 0, SEEK_SET);
            fwrite(&MBR, sizeof(mbr), 1, file);
        }else{
            cout << "--VALOR DELETE INCORRECTO--" << endl << endl;
            return;
        }
        cout << "Particion a eliminada " << auxp.part_name << " de tipo:" << auxp.part_type <<endl;
    }
    this->printDisco(file);
}

void fdisk::modificarParticion() {
    FILE *file;
    file = fopen(this->path.c_str(), "rb+"); // Se abre con permisos mixtos
    if (file == NULL) {
        cout << endl <<"----ERROR: EL DISCO NO EXISTE-----" << endl;
        return;
    }
    mbr MBR;
    int pos;
    fseek(file, 0, SEEK_SET);
    fread(&MBR, sizeof(mbr), 1, file); // Se recupera lo que hay en el archivo en MBR, del tamaño del struct mbr
    ebr aux;
    particion auxp;
    bool otra;
    bool pe = false;
    bool l = false;
    for (int i = 0; i < 4; ++i) {
        if (MBR.mbr_partitions[i].part_name == this->name){
            if (this->add > 0 && MBR.mbr_partitions[i+1].part_status != '0'){
                cout << endl << "-- NO EXISTE ESPACIO DESPUES DE LA PARTICION --" << endl;
                return;
            }
            pe = true;
            pos = i;
            auxp = MBR.mbr_partitions[i];
            break;
        }
        if (MBR.mbr_partitions[i].part_type == 'e'){
            fseek(file, MBR.mbr_partitions[i].part_start, SEEK_SET);
            fread(&aux, sizeof(ebr), 1, file);
            do{
                if(aux.part_name == this->name){
                    if (aux.part_next > 0){
                        cout << endl << "-- NO EXISTE ESPACIO DESPUES DE LA PARTICION --" << endl;
                        return;
                    }
                    auxp = MBR.mbr_partitions[i];
                    l = true;
                    break;
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
        if (l){
            break;
        }
    }
    if (!pe && !l){
        cout << endl << "-- NO EXISTE LA PARTICION EN EL DISCO --" << endl << endl;
        return;
    }
    int numsize;
    if(this->u == "k"){ //kb
        numsize =  this->add * 1024;
    }else if(this->u == "b"){
        numsize = this->add;
    }else if(this->u == "m"){ //mb
        numsize =  this->add * 1024 * 1024;
    }else{
        cout << endl << "---UNIDADES DE SIZE INCORRECTAS---" << endl;
        return;
    }
    if (l){ // si la particion es logica
        if (this->add > 0){ //agregar espacio
            ebr temp;
            fseek(file, auxp.part_start, SEEK_SET);
            fread(&temp, sizeof(ebr), 1, file);
            int contador = 0;
            if (aux.part_size + numsize > auxp.part_size){
                cout << endl << "--- LA PARTICION LOGICA MAS LO AGREGADO, ES MAS GRANDE QUE LA EXTENDIDA ---" << endl;
                return;
            }
            while (temp.part_next != -1){
                if (temp.part_status != '0'){
                    contador += temp.part_size;
                }
                fseek(file, temp.part_next, SEEK_SET);
                fread(&temp, sizeof(ebr), 1, file);
            }
            if (temp.part_status != '0'){
                contador += temp.part_size;
            }
            contador = auxp.part_size - contador;
            if (numsize > contador){
                cout << endl << "--- YA NO HAY ESPACIO EN LA EXTENDIDA PARA ESTA PARTICION CON LO QUE SE AGREGA ---" << endl;
                return;
            }
            aux.part_size = aux.part_size + numsize; // agregado su espacio
        }else{ //quitar espacio
            numsize = numsize * -1;
            if (numsize > aux.part_size){
                cout << endl << "-- ERROR : NO QUEDARA ESPACIO POSITIVO EN LA PARTICION -- " << endl;
                return;
            }
            aux.part_size = aux.part_size - numsize; //quitado su espacio
        }
        fseek(file, aux.part_start - sizeof(ebr), SEEK_SET);
        fwrite(&aux, sizeof(ebr), 1, file);
        cout << "-- Particion logica modificada: " << aux.part_name << endl;
    }
    if(pe){ // si la particion es primario o extendida
        if (this->add > 0){ //agregar espacio
            int disponible = 0;
            for (int i = 0; i < 4; ++i) {
                if (MBR.mbr_partitions[i].part_status != '0'){
                    disponible += MBR.mbr_partitions[i].part_size;
                }
            }
            disponible = MBR.mbr_tamano - disponible;
            if(MBR.mbr_tamano < auxp.part_size + numsize){
                cout << endl << "-- PARTICION MAS AGREGADO MAS GRANDE QUE EL DISCO -- " << endl;
                return;
            }
            if (disponible < numsize){
                cout << endl << "-- EL DISCO NO TIENE ESPACIO DISPONIBLE, PARA ESTA PARTICION CON LO QUE SE LE AGREGA --" << endl;
                return;
            }
            auxp.part_size = auxp.part_size + numsize;
            MBR.mbr_partitions[pos] = auxp;
        }else{ // quitar espacio
            numsize = numsize * -1;
            if (numsize > auxp.part_size){
                cout << endl << "-- ERROR : NO QUEDARA ESPACIO POSITIVO EN LA PARTICION  -- " << endl;
                return;
            }
            auxp.part_size = auxp.part_size - numsize;
            MBR.mbr_partitions[pos] = auxp;
        }
        fseek(file, 0, SEEK_SET);
        fwrite(&MBR, sizeof(mbr), 1, file);
        cout << "-- Particion modificada: " << auxp.part_name << " de tipo:" << auxp.part_type <<endl;
    }
    this->printDisco(file);
}

void fdisk::printDisco(FILE* arch) {
    mbr MBR;
    ebr aux;
    fseek(arch, 0, SEEK_SET);
    fread(&MBR,sizeof(mbr),1,arch);
    bool otra = false;
    cout << "-----------------------------DATOS DEL DISCO--------------------------------" << endl;
    cout << "MBR SIGNATURE: "<< MBR.mbr_disk_signature << endl;
    cout << "MBR SIZE: "<< MBR.mbr_tamano << endl;
    cout << "FECHA: "<< MBR.mbr_fecha_creacion << endl;
    cout << "FIT: " << MBR.disk_fit << endl;
    for (int i = 0; i < 4; ++i) {
        /*if (MBR.mbr_partitions[i].part_status == '0'){
            continue;
        }*/
        cout << "PARTICION : "<< i + 1 << endl;
        cout << "PARTICION STATUS : "<< MBR.mbr_partitions[i].part_status << endl;
        cout << "PARTICION TYPE : "<< MBR.mbr_partitions[i].part_type << endl;
        cout << "PARTICION FIT : "<< MBR.mbr_partitions[i].part_fit << endl;
        cout << "PARTICION START : "<< MBR.mbr_partitions[i].part_start << endl;
        cout << "PARTICION SIZE : "<< MBR.mbr_partitions[i].part_size << endl;
        cout << "PARTICION NAME : "<< MBR.mbr_partitions[i].part_name << endl;
        if (MBR.mbr_partitions[i].part_type == 'e'){
            fseek(arch, MBR.mbr_partitions[i].part_start, SEEK_SET);
            fread(&aux, sizeof(ebr), 1, arch);
            cout << "---------------------" << endl;
            cout << "EBR" << endl;
            int s = 0;
            do{
                s++;
                cout << s << endl;
                if (aux.part_status != '0'){
                    cout << "\tNAME " << aux.part_name << endl;
                    cout << "\tSIZE " << aux.part_size << endl;
                    cout << "\tNEXT " << aux.part_next << endl;
                }
                if (aux.part_next > 0){
                    fseek(arch, aux.part_next, SEEK_SET);
                    fread(&aux, sizeof(ebr), 1, arch);
                    otra = true;
                    cout << "--" << endl;
                }else{
                    otra = false;
                }
            }while(otra);
            cout << "----------------------";
        }
        cout << endl;
    }
    fclose(arch);
}

int fdisk::fitex(char fit) {
    if (fit == 'b')
        return 1;
    else if(fit == 'f')
        return 2;
    else if (fit == 'w')
        return 3;
    return 0;
}

string fdisk::ToLower(string cadena) {
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}