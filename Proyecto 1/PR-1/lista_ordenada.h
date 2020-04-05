#include "lista.h"
/**TDA Lista Ordenada**/

typedef struct lista_ordenada
{
    unsigned int cantidad_elementos;
    TLista lista;
} * TListaOrdenada;

/** Servicios del TDA Lista Ordenada **/

/**
* Crea y retorna una lista ordenada vacía. El orden de los elementos insertados estara dado por la función de
* comparación f
* @param f la función f devuelve -1 si el orden del primer argumento
* es menor que el orden del segundo, 0 si el orden es el mismo,
* y 1 si el orden del primer argumento es mayor que el orden del segundo.
* @return estructura de lista ordenada vacía
*/
TListaOrdenada crear_lista_ordenada(int (*f)(void *,void *));

/**
* Agrega un elemento en la posición correspondiente de la lista, de modo que la misma quede siempre ordenada de
* forma ascendente.
* @param lista lista donde se insertará un elemento
* @param elem elemento a insertar
* @return verdadero si procede con éxito, falso en caso contrario.
*/
int lo_insertar(TListaOrdenada lista, TElemento elem);

/**
* Elimina el elemento de una posición de la lista. Reacomoda la lista adecuadamente al eliminar en posiciones intermedias.
* @param lista lista de donde se eliminará
* @param pos posición de la lista
* @return verdadero si procede con éxito, falso en caso contrario.
*/
int lo_eliminar(TListaOrdenada lista, TPosicion pos);

/**
* Retorna la cantidad de elementos de la lista.
* @param lista lista a consultar cantidad de elementos
* @return cantidad de elementos de la lista
*/
int lo_size(TListaOrdenada lista);

/**
* Retorna la primer posicion de la lista.
* @param lista lista a examinar
* @return La primera posición de una lista.
*/
TPosicion lo_primera(TListaOrdenada lista);

/**
* Retorna la última posición de la lista.
* @param lista lista a examinar
* @return la última posición de una lista
*/
TPosicion lo_ultima(TListaOrdenada lista);

/**
* Retorna la posición siguiente a otra en la lista
* @param lista lista a examinar
* @param pos posición de lista
* @return la posición siguiente a pos en la lista.
*/
TPosicion lo_siguiente(TListaOrdenada lista, TPosicion pos);
