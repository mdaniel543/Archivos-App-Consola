%{
//Area de encabezado y codigo

#include "scanner.h"

#include <iostream>


extern int yylineno;
extern int columna;
extern int linea;
extern char *yytext;

int yyerror(const char* mens)
{
    std::cout << mens <<" "<<yytext<<" Linea: "<<linea<<" Columna: "<<columna<< std::endl;
    return 0;
}

%defines "parser.h"
%output "parser.cpp"
%error-verbose
%locations

%union{
char TEXT [256];
}


//Terminales
