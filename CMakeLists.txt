cmake_minimum_required(VERSION 3.20)
project(MIA_Proyecto1_201709450)

set(CMAKE_CXX_STANDARD 23)

add_executable(MIA_Proyecto1_201709450 main.cpp "Administracion Discos/mkdisk.cpp" "Administracion Discos/mkdisk.h" "Administracion Usuarios/login.cpp" "Administracion Usuarios/login.h" "Administracion Carpetas y Permisos/chmod.cpp" "Administracion Carpetas y Permisos/chmod.h" Reportes/name.cpp Reportes/name.h "Analizador/Interprete.cpp" "Analizador/Interprete.h" Discos/estructuras.cpp Discos/estructuras.h "Administracion Discos/rmdisk.cpp" "Administracion Discos/rmdisk.h" Script/exec.cpp Script/exec.h "Administracion Carpetas y Permisos/pause.cpp" "Administracion Carpetas y Permisos/pause.h" "Administracion Discos/fdisk.cpp" "Administracion Discos/fdisk.h" "Administracion Discos/mount.cpp" "Administracion Discos/mount.h")
