#include <stdlib.h>
#include <stdio.h>
#include "tabla_simbolos.h"
#include "lex.yy.h"
#include "gramatica.tab.h"
#include "estandar.h"

int main(int argc, char** argv){
    //inicializacion
    inicializar_tabla();

    //establece entrada
    if(argc == 1){
        set_entrada(stdin);
        help();
        printf(PROMPT);
    }else{
        for(int i=argc-1; i>0; i--){
            load(argv[i]);
        }
    }
    yyparse();
    quit();
    return 0;
}