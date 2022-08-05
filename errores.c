#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errores.h"
#include "lex.yy.h"

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"



char* crear_str_error(int argc, char** concatenar);

/*
* Funcion que avisa de los errores
* Los cast de void* no son seguros => Es responsabilidad de quien use el modulo 
* usar el parametro info tal y como se indica en el .h
*/

void avisar_error(int error ,int argc, char** info){
    char* mensaje = crear_str_error(argc,info);
    char* error_txt = NULL;
    switch (error){
        case UNEXPECTED_CHAR:
            error_txt = "UNEXPECTED_CHAR";
        break;
        case CAN_NOT_OPEN_FILE:
            error_txt = "CAN_NOT_OPEN_FILE";
        break;
        case SINTAX_ERROR:
            error_txt = "SINTAX_ERROR";
        break;
        case WRONG_ARG_NUM:
            error_txt = "WRONG_ARG_NUM";
        break;
        case FUN_DOESNT_EXIST:
            error_txt = "FUN_DOESNT_EXIST";
        break;
        case WARNING_OVERRIDE:
            error_txt = ANSI_COLOR_MAGENTA"WARNING_OVERRIDE";
        break;
        case UNINITIALIZED_VAR:
            error_txt = "UNINITIALIZED_VAR";
        break;
        case INVALID_OPERAND:
            error_txt = "INVALID_OPERAND";
        break;
        default:
            error_txt = "DESCONOCIDO";
        break;
    }
    printf(ANSI_COLOR_RED"[%d][%d]"ANSI_COLOR_CYAN"-> %s\n"ANSI_COLOR_RESET, error,yylineno,error_txt);
    if (mensaje != NULL) printf(ANSI_COLOR_CYAN"\t %s\n"ANSI_COLOR_RESET, mensaje);
    free(mensaje);
}

char* crear_str_error(int argc, char** concatenar){
    if(argc<=0) return NULL;
    int len = 1;
    char* resultado=NULL;
    for(int i=0; i<argc; i++){
        len+=strlen(concatenar[i]);
    }
    resultado = (char*)malloc(len*sizeof(char));
    if(resultado==NULL) return NULL;
    resultado[len-1]='\0';
    int pos=0;
    for(int i=0; i<argc; i++){
        strcpy(resultado+pos,concatenar[i]);
        pos+=strlen(concatenar[i]);
    }
    return resultado;
}