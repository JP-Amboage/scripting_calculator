#ifndef ERRORES_H
#define ERRORES_H

/*
* Sistema de notificacion de errores
*/

/*
* Codigos de error
*/
#define UNEXPECTED_CHAR 10001
#define CAN_NOT_OPEN_FILE 10002
#define SINTAX_ERROR 10003
#define WRONG_ARG_NUM 10004
#define FUN_DOESNT_EXIST 10005
#define WARNING_OVERRIDE 10006
#define UNINITIALIZED_VAR 10007
#define INVALID_OPERAND 10008

/**
* Funcion para notificar un error (imprimira un aviso)
* @param error codigo del error
* @param argc numero mensajes
* @param info mensajes de error
*/
void avisar_error(int error ,int arg, char** info);

#endif