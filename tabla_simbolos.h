#ifndef TABLA_SIMBOLOS_H
#define TABLA_SIMBOLOS_H

/*
* TABLA DE SIMBOLOS (singleton)
*/

struct funcion {
    double (*fun_ptr)();
    char n_args;
    char* mod_path; //NULL si es una funcion estandar
};

struct comando {
    void (*fun_ptr)();
    char n_args;
};

struct plugin {
    void* plugin;
    char* path;
};

typedef union {
    double val;
    struct funcion funcion;
    struct comando comando;
    struct plugin plugin;
} contenido;

typedef struct {
    char* clave;
    int tipo; 
    contenido valor;
} comp_lexico;


/**
* Funcion que busca en la tabla de simbolos la cadena de texto
* lexema. Si está devuelve su código. Si no está la inserta
* y le asigna el codigo correspondiente a los identificadores
* @param lexema cadena de texto a buscar/insertar
*/
comp_lexico* buscar_comp_lexico(char *lexema);

/**
* Funcion para inicializar la tabla
*/
void inicializar_tabla();

/**
* Funcion para imprimir la tabla (en orden alfabetico)
*/
void print_tabla();

/**
* Funcion para imprimir los registro de tipo tipo
* de la tabla (en orden alfabetico)
* @param tipo
*/
void print_tabla_tipo(int tipo);


/**
* Funcion para suprimir un registro
* de la tabla 
* @param registro: comp_lexico a suprimir
*/
void eliminar_registro(comp_lexico registro);


/**
* Funcion para borrar la tabla 
*/
void borrar_tabla();

/**
* Funcion para salvar el workspace 
* @param filename: nombre del fichero en el que se guardara el workspace
* return 1 si exito -1 si hubo error
*/
int save_workspace(char* filename);

#endif