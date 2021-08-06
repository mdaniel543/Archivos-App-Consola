%option case-insensitive
%option yylineno
%option header-file="scanner.h"
%option outfile="scanner.cpp"

%{
#include <iostream>
#include <string>
#include "parser.h"

int yylex();
void yyerror(const char *s);
int linea = 0;
int columna = 1;
int anterior = 0;

%}
%option noyywrap
%option nounput

DIGITO [0-9]
LETRA [a-zA-ZñÑ]

NUMERO {DIGITO}+("."{DIGITO}+)?
IDENTIFICADOR {LETRA}({LETRA}|{DIGITO}|"_"|".")*
CADENA "\""[^"\""]*"\""
RUTA (\/{IDENTIFICADOR})+
NEGATIVO ["-"][{DIGITO}]
DELIMITADOR [\t\r\f" "]

%%
"MKDISK" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return PRmkdisk;}
"=" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return IGUAL;}
"-" {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return GUION;}

{CADENA} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return CADENA;}
{NUMERO} {columna=columna+ anterior;  anterior = strlen(yytext); strcpy(yylval.TEXT, yytext); return NUMERO;}
{NEGATIVO} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return NEGATIVO;}
{IDENTIFICADOR} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return ID;}
{RUTA} {columna=columna+ anterior;  anterior = strlen(yytext);strcpy(yylval.TEXT, yytext);return RUTA;
{DELIMITADOR}   {}
"\n" {linea++; columna=0; anterior=0;}
.               {printf("ERROR LEXICO %s\n", yytext);}
%%