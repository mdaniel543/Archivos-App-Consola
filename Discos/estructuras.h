//
// Created by daniel on 11/08/21.
//

#ifndef MIA_PROYECTO1_201709450_ESTRUCTURAS_H
#define MIA_PROYECTO1_201709450_ESTRUCTURAS_H
#include <ctime>

typedef struct{
    char part_status;
    char part_type;
    char part_fit;
    int part_start;
    int part_size;
    char part_name[16];
}particion;

typedef struct{
    int mbr_tamano;
    char mbr_fecha_creacion[20];
    int mbr_disk_signature;
    char disk_fit[1];
    particion mbr_partitions[4];
}mbr;


class estructuras {
public:
    estructuras();
};

#endif //MIA_PROYECTO1_201709450_ESTRUCTURAS_H
