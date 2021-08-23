//
// Created by daniel on 12/08/21.
//

#include "mount.h"

mount::mount() {
}

void mount::montarParticion(string path, string name) {
    FILE *file;
    file = fopen(path.c_str(), "rb+"); // Se abre con permisos mixtos
    if (file == NULL) {
        cout << endl <<"----ERROR: EL DISCO NO EXISTE-----" << endl;
        return;
    }
    mbr MBR;
    ebr aux;
    bool otra, l = false, pe = false;
    fseek(file, 0, SEEK_SET);
    fread(&MBR, sizeof(mbr), 1, file); // Se recupera lo que hay en el archivo en MBR, del tamaño del struct mbr
    for (int i = 0; i < 4; ++i) {
        if (MBR.mbr_partitions[i].part_status == '0'){
            continue;
        }
        if (MBR.mbr_partitions[i].part_name == name){
            pe = true;
            break;
        }
        if (MBR.mbr_partitions[i].part_type == 'e'){
            fseek(file, MBR.mbr_partitions[i].part_start, SEEK_SET);
            fread(&aux, sizeof(ebr), 1, file);
            do{
                if(aux.part_name == name){
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
    fclose(file);
    if (!pe && !l){
        cout << endl << "-- NO EXISTE LA PARTICION EN EL DISCO --" << endl << endl;
        return;
    }
    int i; // QUEMO LA POSICION DEL DISCO
    bool existe = false;
    for (i = 0; i < 10; ++i) {
        if(discos[i].estado == 0){
            discos[i].estado = 1;
            discos[i].path = path;
            discos[i].letra = 49;
            numeroparticion++;
            existe = true;
            break;
        } else if (discos[i].path == path){
            existe = true;
            break;
        }
    }
    if (!existe){
        cout << "-- No hay espacio para montar otro disco --" << endl;
        return;
    }
    for (int j = 0; j < 24; ++j) { /// MONTO LA PARTICION
        if (discos[i].particiones[j].estado == 0){
            discos[i].particiones[j].numero = numeroparticion;
            discos[i].particiones[j].nombre = name;
            discos[i].particiones[j].estado = 1;
            char con = discos[i].letra + '0'; // lo convierto al ascii
            discos[i].particiones[j].id = to_string(carnet) + to_string(discos[i].particiones[j].numero) + con;
            discos[i].letra++;
            break;
        }
    }
    // mando a imprimir mensaje
    this->imprimirMontajes();
}


void mount::imprimirMontajes() {
    char esc_char = 27;
    cout << endl;
    for (int i = 0; i < 10; ++i) {
        if (discos[i].estado == 1){
            for (int j = 0; j < 24; ++j) {
                if (discos[i].particiones[j].estado == 1){
                    cout << esc_char << "[1m" << discos[i].path << "|" << discos[i].particiones[j].nombre << "|" << discos[i].particiones[j].id << esc_char << "[0m" <<endl;
                }
            }
        }
    }
    cout << endl;
}
