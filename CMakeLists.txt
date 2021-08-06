cmake_minimum_required(VERSION 3.20)
project(MIA_Proyecto1_201709450)

set(CMAKE_CXX_STANDARD 23)

add_executable(MIA_Proyecto1_201709450 main.cpp "Administracion Discos/mkdisk.cpp" "Administracion Discos/mkdisk.h" "Administracion Usuarios/login.cpp" "Administracion Usuarios/login.h" "Administracion Carpetas/chmod.cpp" "Administracion Carpetas/chmod.h" Reportes/name.cpp Reportes/name.h "Analizador /Interprete.cpp" "Analizador /Interprete.h")
