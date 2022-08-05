#include <stdio.h>
#include "tabla_simbolos.h"
#include "lex.yy.h"
#include <dlfcn.h>
#include "gramatica.tab.h"
#include "errores.h"
#include "estandar.h"

//comando para salir del programa
void quit(){
    //imprime mensaje
    if(yyin==stdin){ 
        char ans;
        //tiene que ser dentro del if
        //para tener acceso a yyin
        yylex_destroy();
        //no puede ser despues porque procesaria la respuesta
        //recogida en scanf
        printf("- Desea guardar el workspace actual? [N|s]\n");
        scanf("%c",&ans);
        if(ans == 's' || ans == 'S'){
            if(save_workspace("workspace.jp")){
                printf("- El workspace se ha guardado con exito\n");
                printf("- NOTA: algunas variables pueden haber perdido precision\n");
                printf("- Puedes restaurar el workspace escribiendo \"load :workspace.jp\"\n");
                printf("- Puedes restaurar el workspace ejecutanso \"./interprete workspace.jp\"\n");
            }else{
                printf("- Lo sentimos se ha perdido el workspace\n");
            }
        }
        printf("Adios :)\n");
    }else{
        yylex_destroy();
    }
    //libera memoria
    borrar_tabla();
    //sale
    exit(0);
}

//comando para imprimir las variables del workspace
void workspace(){
    printf("============================\n");
    printf("====== WORKSPACE VARs ======\n");
    print_tabla_tipo(VAR);
    printf("============================\n");

}

//comando para cargar ficheros
void load(char* filename){
    //aviso de load, me gusta mas como queda no darlo
    //printf("LOAD: %s\n",filename);
    //abrir fichero en modo lectura
    FILE* fp = fopen(filename,"r");
    //comprobacion + aviso de errror
    if (fp == NULL){
        char** err=(char**)malloc(2*sizeof(char*));
        err[0]="No se pudo abrir el fichero: ";
        err[1]= filename;
        avisar_error(CAN_NOT_OPEN_FILE,2,err);
        free(err);
        return; //termina el comando
    }
    //cambia la entrada al fichero
    set_entrada(fp);
}

//comando para resetear el workspace
void reset(){
    borrar_tabla();
    inicializar_tabla();
    if(yyin==stdin) printf("Workspace eliminado\n");
}

//comando para importar una funcion de un modulo
void import_fun(char* filename, char* funname){
    if(yyin==stdin){
        printf("- Importing %s() from module %s\n",funname,filename);
    }
    void *plugin;
    double (*fun)();
    char* result = NULL;
    //abrir el plugin
    plugin = dlopen(filename, RTLD_LAZY);
    //comprobacion + error
    if (!plugin){
        char** err=(char**)malloc(2*sizeof(char*));
        err[0]="No se pudo cargar el plugin: ";
        err[1]= filename;
        avisar_error(CAN_NOT_OPEN_FILE,2,err);
        free(err);
        return;//fin comando
    }
    //carga de la funcion
    fun = dlsym(plugin, funname);
    //comprobacion de error + notificacion
    result = dlerror();
    if (result){
        char** err=(char**)malloc(4*sizeof(char*));
        err[0]="No se pudo cargar la funcion ";
        err[1]= funname;
        err[2]= " del plugin ";
        err[3]= filename;
        avisar_error(FUN_DOESNT_EXIST,4,err);
        free(err);
        return;//fin comando
    }
    //carga de la nueva funcion en la tabla 
    comp_lexico* comp = buscar_comp_lexico(funname);

    //si el identificador esta en uso Warning
    if(comp->tipo != -1){
        char** err=(char**)malloc(4*sizeof(char*));
        err[0]="El identificador ";
        err[1]= funname;
        err[2]= " ya estaba en uso y ha sido sobreescrito. ";
        err[3]= "\nSi era un comando o una constante puedes recuperar el simbolo original con \"reset\".";
        avisar_error(WARNING_OVERRIDE,4,err);
        free(err);
        if(comp->tipo==MOD){
            if(comp->valor.plugin.path!=NULL) free(comp->valor.plugin.path);
        }else if(comp->tipo==FUN){
            if(comp->valor.funcion.mod_path!=NULL) free(comp->valor.funcion.mod_path);
        }
    }
    //completar campos del registro en la tabla
    comp->tipo = FUN;
    struct funcion funcion;
    funcion.fun_ptr=fun;
    funcion.n_args=1;
    funcion.mod_path = strdup(filename);
    comp->valor.funcion = funcion;
}

//comando para importar un modulo
void import_mod(char* filename, char* modname){
    if(yyin==stdin){
        printf("- Importing module %s as %s\n",filename,modname);
    }
    void *plugin;
    //abrir modulo
    plugin = dlopen(filename, RTLD_LAZY);

    //comprobar error + notificacion
    if (!plugin){
        char** err=(char**)malloc(2*sizeof(char*));
        err[0]="No se pudo cargar el plugin: ";
        err[1]= filename;
        avisar_error(CAN_NOT_OPEN_FILE,2,err);
        free(err);
        return;
    }  

    //cargamos el modulo en la tabla
    comp_lexico* comp = buscar_comp_lexico(modname);

    //comprobacion de si el nombre esta en uso + warning
    if(comp->tipo != -1){
        char** err=(char**)malloc(4*sizeof(char*));
        err[0]="El identificador ";
        err[1]= modname;
        err[2]= " ya estaba en uso y ha sido sobreescrito. ";
        err[3]= "\nSi era un comando o una constante puedes recuperar el simbolo original con \"reset\".";
        avisar_error(WARNING_OVERRIDE,4,err);
        free(err);
        if(comp->tipo==MOD){
            if(comp->valor.plugin.path!=NULL) free(comp->valor.plugin.path);
        }else if(comp->tipo==FUN){
            if(comp->valor.funcion.mod_path!=NULL) free(comp->valor.funcion.mod_path);
        }
        
    }
    //establecer campos del registro
    comp->tipo = MOD;
    comp->valor.plugin.plugin = plugin;
    comp->valor.plugin.path = strdup(filename);
}

//funcion ayuda
void help(){
    printf("==================================================================\n");
    printf("============ Bienvenido a la ayuda de la calculadora =============\n");
    printf("==================================================================\n");
    printf("- Para terminar el programa escriba \"quit\"\n");
    printf("- Para ver las variables del espacio de trabajo escriba \"workspace\"\n");
    printf("- Para limpiar el espacio de trabajo escriba \"reset\"\n");
    printf("- Para cargar un fichero de instrucciones escriba \"load :filename\"\n");
    printf("- Para importar un modulo externo escriba \"import_mod :path_modulo :nombre_modulo\"\n");
    printf("\t- Usted puede dar al modulo externo el nombre que quiera\n");
    printf("\t- Para ejecutar funciones del modulo escriba \"nombre_modulo.funname(expresion)\"\n");
    printf("- Para importar una funcion de un modulo externo escriba \"import_mod :path_modulo :nombre_funcion\"\n");
    printf("\t- Podra ejecutar la funcion importada con la sintaxis habitual: \"funname(expresion)\"\n");
    printf("- Usted puede realizar operaciones aritmeticas habituales\n");
    printf("- Dispone de un amplio surtido de funciones matematicas.\n");
    printf("\t- Para invocar una funcion escriba \"funname(expresion)\".\n");
    printf("- Las constantes PI y E estan disponibles en la calculadora\n");
    printf("- Escriba \";\" al final de una cuenta para que sea silenciosa\n");
    printf("- Puede dejar mensajes o comentarios escribiendo \"#comentario\"\n");
    printf("==================================================================\n");
    printf("====== FIN: si desea volver a ver esta ayuda escriba \"help\" ======\n");
    printf("==================================================================\n");
}
