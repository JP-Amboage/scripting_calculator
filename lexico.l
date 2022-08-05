%{
#include "errores.h"
#include "tabla_simbolos.h"
#include "gramatica.tab.h"
%}

/**
* directivas de flex para
* evitar que se genere codigo
* para funciones que no usamos
*/
%option nounput
%option noinput

/***CONTADOR DE LINEAS***/
%option yylineno

NEWLINE_ \n
LETTER_ [A-Za-z]|"_"
DECIMAL_DIGIT_ [0-9]

SEMICOLON_ ";"

IDENTIFIER_ {LETTER_}({LETTER_}|{DECIMAL_DIGIT_})*

DIGITS_ {DECIMAL_DIGIT_}+

EXPONENT_  ("e"|"E")("+"|"-")?{DIGITS_}

NUMBER_ ({DIGITS_}"."{DIGITS_}|"."{DIGITS_}|{DIGITS_}){EXPONENT_}?

ARGUMENTO_ ":"[^(" "|"%"|"\n")]+

MOD_FUN_ "."{IDENTIFIER_}



%%
{NEWLINE_}                              {return '\n';}
{IDENTIFIER_}                           {
                                        yylval.info = buscar_comp_lexico(yytext);
                                        if(yylval.info->tipo == -1) return VAR;
                                        return yylval.info->tipo;
                                        }
"="|"+"|"-"|"*"|"/"|"^"|"("|")"|";"        {return yytext[0];}
{NUMBER_}                               {yylval.val = atof(yytext); return NUM;}

{ARGUMENTO_}                            {yylval.str=strdup(yytext+1); return ARG;}

{MOD_FUN_}                            {yylval.str=strdup(yytext+1); return MOD_FUN;}

"#".*                                   {printf("%s\n",yytext);}

" "|"\t"
. {char** mensaje= (char**)malloc(2*sizeof(char*));mensaje[0]="No se esperaba el caracter ";mensaje[1]=yytext; avisar_error(UNEXPECTED_CHAR,2,mensaje);free(mensaje);}

%%

void set_entrada(FILE* entrada){
    yyin = entrada;
    yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
}

int yywrap(){
    if(yyin==stdin) return 0;
    fclose(yyin);
    yypop_buffer_state();
    if(!YY_CURRENT_BUFFER) set_entrada(stdin);
    if(yyin == stdin) printf(PROMPT);
    return 0;
}