#ifndef ESTANDAR_H
#define ESTANDAR_H

/**
* 
* LIBRERIA "ESTANDAR" de la calculadora
* exporta los comandos y funciones por defecto
*
*/

#include <math.h>
#define PI M_PI
#define E M_E

//salir del interprete
void quit();
//imprime las variables y sus valores
void workspace();
//carga un fichero con instrucciones
void load(char* filename);
//imprime un texto de ayuda
void help();
//importa una funcion de un modulo externo
void import_fun(char* filename, char* funname);
//importa un modulo externo
void import_mod(char* filename, char* modname);
//reinicia el workspace
void reset();

#endif