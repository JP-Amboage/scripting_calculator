#include "tabla_simbolos.h"
#include "abb.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "estandar.h"
#include "gramatica.tab.h"
#include "errores.h"

/*
* TABLA DE SIMBOLOS (singleton)
*/

struct init_fun {     
    char *fname;
    double (*fnct)();
    char n_args;
};

struct init_fun mis_funs[]={
    {"floor", floor, 1},
    {"ceil", ceil, 1},
    {"tan", tan,1},
    {"atan", atan,1},
    {"sin", sin, 1},
    {"asin", asin,1},
    {"sinh", sinh,1},
    {"cos", cos, 1},
    {"acos", acos,1},
    {"cosh", cosh,1},
    {"exp", exp,1},
    {"log", log,1},
    {"log10", log10,1},
    {"abs", fabs,1},
    {"sqrt", sqrt,1},
    {NULL, NULL, 0}
};

struct init_com {     
    char *cname;
    void (*fnct)();
    char n_args;
};

struct init_com mis_comandos[]={
    {"quit", quit, 0},
    {"help", help, 0},
    {"workspace", workspace, 0},
    {"load", load, 1},
    {"import_fun", import_fun, 2},
    {"import_mod", import_mod, 2},
    {"reset", reset, 0},
    {NULL, NULL, 0}
};

struct init_cte {     
    char *cname;
    double val;
};

struct init_cte mis_cts[]={
    {"PI", PI},
    {"E", E},
    {NULL, 0}
};



//arbol e que se apoya la tabla (singleton)
abb arbol;


//buscar componente lexico
comp_lexico* buscar_comp_lexico(char *lexema){
    //funcion añadida a la librería abb.h 
    return buscar_insertar(&arbol, lexema);
}

//inicialización de la tabla (singleton)
void inicializar_tabla(){
    //creacion del arbol
    crear_arbol(&arbol);   
    //insercion del contenido inicial de la tabla
    tipoelem comp;
    for(int i=0; mis_funs[i].fname!=NULL;i++){
        comp.clave = strdup(mis_funs[i].fname);
        comp.tipo = FUN;
        comp.valor.funcion.fun_ptr=mis_funs[i].fnct;
        comp.valor.funcion.n_args=mis_funs[i].n_args;
        comp.valor.funcion.mod_path=NULL;
        insertar(&arbol, comp);
    }

    for(int i=0; mis_comandos[i].cname!=NULL;i++){
        comp.clave = strdup(mis_comandos[i].cname);
        comp.tipo = COM;
        comp.valor.comando.fun_ptr=mis_comandos[i].fnct;
        comp.valor.comando.n_args=mis_comandos[i].n_args;
        insertar(&arbol, comp);
    }

    for(int i=0; mis_cts[i].cname!=NULL;i++){
        comp.clave = strdup(mis_cts[i].cname);
        comp.tipo = CTE;
        comp.valor.val=mis_cts[i].val;
        insertar(&arbol, comp);
    }
}

//borrado de la tabla
void borrar_tabla(){
    destruir_arbol(&arbol);
}

//imprime la tabla en orden
void print_tabla(){
    printf("========= TABLA SIMBOLOS =========\n");
    inorden(arbol);
    printf("======== FIN TABLA SIMBOLOS =======\n");
}

void eliminar_registro(comp_lexico registro){
    suprimir(&arbol, registro);
}

//imprime la tabla en orden
void print_tabla_tipo(int tipo){
    switch (tipo) {
    case VAR:
    case CTE:
        printf("| name\t| value\t\t|\n");
    break;
    case FUN:
    case COM:
        printf("| name\t| num args\t|\n");
    break;
    case MOD:
        printf("| module name\t|\n");
    break;
    default:
        return;
    break;
    }
    printf("----------------------------\n");
    print_tipo(arbol,tipo);
}

//guarda el workspace
int save_workspace(char* filename){
    FILE* fp = fopen(filename,"w");
    //comprobamos si se abrio el fichero 
    if(fp==NULL){
        char** err=(char**)malloc(2*sizeof(char*));
        err[0]="No se pudo crear el fichero para el workspace: ";
        err[1]= filename;
        avisar_error(CAN_NOT_OPEN_FILE,2,err);
        free(err);
        return -1;
    }
    save_arbol(arbol,fp);
    fprintf(fp,"#WORKSPACE RESTAURADO\n");
    return 1;
}
