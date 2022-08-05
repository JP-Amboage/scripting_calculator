%{
#include <math.h>
#include <dlfcn.h>
#include "lex.yy.h"
#include "tabla_simbolos.h"
#include "estandar.h"
#include "errores.h"
#include "string.h"
void yyerror (char const *);
int comprobar_var(comp_lexico* comp);
double aplicar_fun_mod(comp_lexico* comp, char* funname, double val);
int check_num_args(comp_lexico* comp, int nargs);
%}

%code requires{
    #define PROMPT ">>> "
}

%union {
double val; /* Para devolver numeros */
comp_lexico* info; /* Para devolver punteros a la tabla de sımbolos */
char* str;
}

%token <val> NUM /* Numero */
%token <info> CTE FUN VAR COM MOD /* Variable Constante y Funcion */
%token <str> ARG MOD_FUN /* Variable Constante y Funcion */
%type <val> exp

%right '='
%left '-' '+'
%left '*' '/'
%left NEG /* Negacion--menos unario */
%right '^' /* Exponenciacion */

/* A continuacion la gramatica */

%%

input:  %empty
        | input line {if(yyin==stdin)printf(PROMPT);}
;

line:   '\n'            
    | exp '\n'          {printf("\t%f\n",$1);}
    | exp ';' '\n'      
    | error '\n'        {yyerrok;}
    | comand '\n'       
;

comand:
    COM               {   if(check_num_args($1,0)){
                                $1->valor.comando.fun_ptr();
                            }else{
                                YYERROR;
                            }
                        }

    | COM ARG           {   if(check_num_args($1,1)){
                                $1->valor.comando.fun_ptr($2);
                                free($2);
                            }else{
                                free($2);
                                YYERROR;
                            }
                        }
    | COM ARG ARG       {   if(check_num_args($1,2)){
                                $1->valor.comando.fun_ptr($2,$3);
                                free($2);
                                free($3);
                            }else{
                                free($2);
                                free($3);
                                YYERROR;
                            }
                        }
;

exp:    NUM                        {$$ = $1; }
    |   VAR                        {if(comprobar_var($1)){$$=$1->valor.val;}else{YYERROR;}}
    |   VAR '=' exp                {$1->valor.val=$3; $1->tipo=VAR; $$=$1->valor.val;}
    |   FUN '(' exp ')'            {if(check_num_args($1,1)){$$=$1->valor.funcion.fun_ptr($3);}else{YYERROR;}}
    |   CTE                        {$$=$1->valor.val;}
    |   exp '+' exp                {$$=$1+$3;}
    |   exp '-' exp                {$$=$1-$3;}
    |   exp '*' exp                {$$=$1*$3;}
    |   exp '/' exp                {    if($3){
                                            $$=$1/$3;
                                        }else{
                                            char** err = (char**)malloc(1*sizeof(char*));
                                            err[0]="No puedes dividir entre 0";
                                            avisar_error(INVALID_OPERAND, 1, err);
                                            free(err);
                                            YYERROR;
                                        }
                                    }
    |   '-' exp %prec NEG          {$$=-$2;}
    |   exp '^' exp                {$$=pow($1,$3);}
    |   MOD MOD_FUN '(' exp ')'               {$$=aplicar_fun_mod($1,$2,$4); free($2);}
;

/*fin de la gramatica*/
%%

int yyparse(void);

int comprobar_var(comp_lexico* comp){
    if(comp->tipo == VAR) return 1;
    char** err = (char**)malloc(3*sizeof(char*));
    err[0]="La variable ";
    err[1]= comp->clave;
    err[2]= " no esta inicializada";
    avisar_error(UNINITIALIZED_VAR, 3, err);
    free(err);
    eliminar_registro(*comp);
    return 0;
}

int check_num_args(comp_lexico* comp, int nargs){
    short is_valid;
    switch(comp->tipo){
        case COM:
            is_valid=(comp->valor.comando.n_args==nargs);
        break;
        case FUN:
            is_valid=(comp->valor.funcion.n_args==nargs);
        break;
        default:
            is_valid=0;
        break;
    }
    if(is_valid) return 1;
    char** err=(char**)malloc(3*sizeof(char*));
    err[0]="La llamada a ";
    err[1]= comp->clave;
    err[2]= " esperaba otro numero de argumentos. Consulta su uso con help";
    avisar_error(WRONG_ARG_NUM,3,err);
    free(err);
    return 0;
}

double aplicar_fun_mod(comp_lexico* comp, char* funname, double val){
    double (*fun)();
    char* result = NULL;
    fun = dlsym(comp->valor.plugin.plugin, funname);
    result = dlerror();
    if (result){
        char** err=(char**)malloc(5*sizeof(char*));
        err[0]="No se pudo cargar la funcion ";
        err[1]= funname;
        err[2]= " del plugin ";
        err[3]= comp->clave;
        err[4]= ". La operacion devolvera nan. ";
        avisar_error(FUN_DOESNT_EXIST,5,err);
        free(err);
        return NAN;
    }
    return fun(val);
}

void yyerror (char const *s) {
    //printf("-> %s: %d\n",s,yylineno);
    char** err = (char**)malloc(1*sizeof(char*));
    err[0]=strdup(s);
    avisar_error(SINTAX_ERROR,1,err);
    free(err[0]);
    free(err);
}
