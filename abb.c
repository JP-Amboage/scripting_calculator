#include <stdlib.h>
#include <string.h>

// Importar aqui abb.h permite no repetir la definicion de 
// tipoelem. Si no lo hiciesemos tendriamos que copiarla
// en la implementacion. 
#include "abb.h"


///////////////////////// ESTRUCTURAS DE DATOS

struct celda {
	tipoelem info;
	struct celda *izq, *der;
};

//////////////////////// FUNCIONES


/////////////////////////// INICIO PARTE MODIFICABLE

/*
 * Extraer la clave de una celda
 */
tipoclave _clave_elem(tipoelem * E){
    return (E)->clave;
}

/* Esta funcion puente nos permite modificar el tipo de
 * de datos del TAD sin tener que cambiar todas las
 * comparaciones del resto de la biblioteca y en su lugar
 * cambiando solo esta.
 */
int _comparar_claves(tipoclave cl1, tipoclave cl2){
    return !strcmp(cl1,cl2)? 0 : strcmp(cl1,cl2)>0 ? 1 : -1;
}

/*
 * Si tipoelem tiene alguna estructura que necesite 
 * destruirse ha de hacerse aqui. El uso de esta funcion
 * permite hacer mas eficiente la destruccion del arbol.
 */
void _destruir_elem(tipoelem *E){
    free(E->clave);
    switch (E->tipo){
            case VAR: //var y cte son iguales
            case CTE:
            //nada que liberar actualmente
            break;
            case FUN:
                //liberamos el char* con path del modulo
                free(E->valor.funcion.mod_path);
            break;
            case MOD:
                //liberamos el char* con path del modulo
                free(E->valor.plugin.path);
            break;
            case COM:
                //nada que liberar actualmente;
            break;
    }
}

/////////////////////////// FIN PARTE MODIFICABLE

// Funcion privada (no esta en el .h)
int _comparar_clave_elem(tipoclave cl, tipoelem E){
	return _comparar_claves(cl, _clave_elem(&E)); 
}

void crear_arbol(abb *A) {
	*A = NULL;
}

unsigned es_vacio(abb A) {
	return A == NULL;
}

abb izq(abb A) {
    return A->izq;
}

abb der(abb A) {
    return A->der;
}

void destruir_arbol(abb *A) {
	if (!es_vacio(*A)) {
		destruir_arbol(&((*A)->izq));
		destruir_arbol(&((*A)->der));
		_destruir_elem(&((*A)->info));
		free(*A);
		*A = NULL;
	}
}

/* Funcion privada para pasar la clave y no tener que
   extraerla del nodo en las llamadas recursivas.
   Esta funcion no aparece en el fichero .h
   */
void _modificar(abb *A, tipoclave cl, tipoelem nodo){
    if (es_vacio(*A)) {
        return;
    } 
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if (comp == 0) {
        (*A)->info = nodo;
    } else if (comp < 0) {
        _modificar(&(*A)->izq, cl, nodo);
    } else {
        _modificar(&(*A)->der, cl, nodo);
    }
}

/* Permite modificar el nodo extrayendo del mismo 
   la clave */
void modificar(abb *A, tipoelem nodo) {
    tipoclave cl = _clave_elem(&nodo);
    _modificar(A,cl,nodo);
}

/* Funcion recursiva para insertar un nuevo nodo 
   en el arbol. Se presupone que no existe un nodo
   con la misma clave en el arbol. */
void insertar(abb *A, tipoelem E) {
    if (es_vacio(*A)) {
        *A = (abb) malloc(sizeof (struct celda));
        (*A)->info = E;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return;
    } 
    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info );
    if (comp > 0 ) {
        insertar(&(*A)->der, E);
    } else {
        insertar(&(*A)->izq, E);
    }
}

/* Funcion privada que permite ...
*/
tipoelem _suprimir_min(abb *A) {
    abb aux;
    tipoelem ele;
    if (es_vacio((*A)->izq)) {
        ele = (*A)->info;
        aux = *A;
        *A = (*A)->der;
        _destruir_elem(&aux->info);
        free(aux);
        return ele;
    } else {
        return _suprimir_min(&(*A)->izq);
	}
}

/* Funcion que permite eliminar un nodo del arbol */
void suprimir(abb *A, tipoelem E) {
    abb aux;
    if(es_vacio(*A)){
       return;
    }
    
    tipoclave cl = _clave_elem(&E);
    int comp = _comparar_clave_elem(cl, (*A)->info);
    if(comp < 0){ //if (E < (*A)->info) {
        suprimir(&(*A)->izq, E);
    } else if (comp > 0){ //(E > (*A)->info) {
        suprimir(&(*A)->der, E);
    } else if (es_vacio((*A)->izq) && es_vacio((*A)->der)) {
        _destruir_elem(&((*A)->info));
        free(*A);
        *A = NULL;
    } else if (es_vacio((*A)->izq)) { 	// pero no es vacio derecha
        aux = *A;
        *A = (*A)->der;
        _destruir_elem(&aux->info);
        free(aux);
    } else if (es_vacio((*A)->der)) { 	//pero no es vacio izquierda
        aux = *A;
        *A = (*A)->izq;
        _destruir_elem(&aux->info);
        free(aux);
    } else { 							//ni derecha ni izquierda esta vacio
        (*A)->info = _suprimir_min(&(*A)->der);
    }
}

unsigned es_miembro(abb A, tipoelem E) {
    return es_miembro_clave(A, _clave_elem(&E));
}

unsigned es_miembro_clave(abb A, tipoclave cl) {
    if (es_vacio(A)) {
        return 0;
    } 
    int comp = _comparar_clave_elem(cl, A->info);
    
    if (comp == 0) { 	//cl == A->info
        return 1;
    } 
    if (comp > 0) {		//cl > A->info
        return es_miembro_clave(A->der, cl);
    } 
    //cl < A->info
    return es_miembro_clave(A->izq, cl);
}

void buscar_nodo(abb A, tipoclave cl, tipoelem *nodo) {
    if (es_vacio(A)) {
        return;
    } 
    int comp = _comparar_clave_elem(cl, A->info);
    
	if (comp == 0) { 		// cl == A->info
    	*nodo = A->info;
	} else if (comp < 0) { 	// cl < A->info
	    buscar_nodo(A->izq, cl, nodo);
	} else { 				// cl > A->info
	    buscar_nodo(A->der, cl, nodo);
	}
}

void info(abb A, tipoelem *E) {
    *E = A->info;
}


/*
* Funcion recursiva que permite imprimir el arbol en orden
*/
void inorden(abb A){
    tipoelem E;
    //comprobamos que A no sea vacio
    if(!es_vacio(A)){
        //hacemos el inorden de la izquierda
        inorden(izq(A));
        //imprimimos el nodo en el que estamos
        info(A,&E);
        printf("<%s>\n", E.clave);
        //hacemos el inorden de la derecha
        inorden(der(A));
    }
}

void print_tipo(abb A, int tipo){
    tipoelem E;
    //comprobamos que A no sea vacio
    if(!es_vacio(A)){
        //hacemos el inorden de la izquierda
        print_tipo(izq(A),tipo);
        //imprimimos el nodo en el que estamos
        info(A,&E);
        if(E.tipo == tipo){
            switch (tipo){
            case VAR:
            case CTE:
                printf("| %s\t| %lf\n",E.clave,E.valor.val);
            break;
            case FUN:
                printf("| %s()\t| %d\t|\n",E.clave,E.valor.funcion.n_args);
            break;
            case MOD:
                printf("| %s\t|\n",E.clave);
            break;
            case COM:
                printf("| %s\t| %d\t|\n",E.clave,E.valor.comando.n_args);
            break;
            default:
                printf("| %s\t| unknown %d\t|\n",E.clave,tipo);
            break;
            }
        }
        //hacemos el inorden de la derecha
        print_tipo(der(A),tipo);
    }
}

/* 
* Funcion recursiva para insertar un nuevo nodo 
* en el arbol si este no existe
*/
tipoelem* buscar_insertar(abb *A, char* lexema) {
    //si llega a donde tocaria insertar
    if (es_vacio(*A)) {
        //reserva memoria
        *A = (abb) malloc(sizeof (struct celda));
        //copia el lexema
        char * lexema_copia;
        lexema_copia = strdup(lexema);
        if(lexema_copia==NULL) return NULL;
        //crea un nuevo nodo
        (*A)->info = (tipoelem) {.clave=lexema_copia} ;
        (*A)->info.tipo = -1;
        (*A)->izq = NULL;
        (*A)->der = NULL;
        return &((*A)->info);
    }
    //si donde llega hay un nodo compara el lexema con la clave del nodo
    tipoclave cl = lexema;
    int comp = _comparar_clave_elem(cl, (*A)->info );
    if(comp == 0){
        //si las claves coinciden devuelve el codigo del lexema
        return &((*A)->info);
    }else if (comp > 0 ) {
        //si las claves no coinciden sigue explorando por la rama correspondiente
       return buscar_insertar(&(*A)->der, lexema);
    } else {
        //si las claves no coinciden sigue explorando por la rama correspondiente
       return buscar_insertar(&(*A)->izq, lexema);
    }
}

void save_arbol(abb A, FILE* file){
    tipoelem E;
    //comprobamos que A no sea vacio
    if(!es_vacio(A)){
        //hacemos el save de la izquierda
        save_arbol(izq(A),file);
        //imprimimos el nodo en el que estamos
        info(A,&E);
        switch (E.tipo){
            case VAR: 
                fprintf(file,"%s=%lf;\n",E.clave,E.valor.val);
            break;
            case CTE:
                //en las ctes no hay que salvar nada
            break;
            case FUN:
                //solo debemos guardar las funs importadas
                if(E.valor.funcion.mod_path!=NULL){
                    fprintf(file,"import_fun :%s :%s\n",E.valor.funcion.mod_path,E.clave);
                }
            break;
            case MOD:
                fprintf(file,"import_mod :%s :%s\n",E.valor.plugin.path,E.clave);
            break;
            case COM:
                //no hay que guardar nada ya que no se pueden importar ni crear comandos
            break;
        }
        //hacemos el save de la derecha
        save_arbol(der(A),file);
    }
}