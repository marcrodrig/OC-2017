#include <stdlib.h>
#include "lista_ordenada.h"
#include "constantes.h"

int (*comparador)(void *,void *);

/**
* Crea y retorna una lista ordenada vacía. El orden de los elementos insertados estara dado por la función de
* comparación f
* @param f la función f devuelve -1 si el orden del primer argumento
* es menor que el orden del segundo, 0 si el orden es el mismo,
* y 1 si el orden del primer argumento es mayor que el orden del segundo.
* @return estructura de lista ordenada vacía
*/
TListaOrdenada crear_lista_ordenada(int (*f)(void *,void *))
{
    TListaOrdenada lista_ordenada = malloc(sizeof(struct lista_ordenada));
    TLista listita = crear_lista();
    lista_ordenada->lista = listita;
    lista_ordenada->cantidad_elementos = 0;
    comparador = f;
    return lista_ordenada;
}

/**
* Agrega un elemento en la posición correspondiente de la lista, de modo que la misma quede siempre ordenada de
* forma ascendente.
* @param lista lista donde se insertará un elemento
* @param elem elemento a insertar
* @return verdadero si procede con éxito, falso en caso contrario.
*/
int lo_insertar(TListaOrdenada lista_ordenada, TElemento elem)
{
    TLista listas = lista_ordenada->lista; /*obtengo lista con elementos.*/
    if (listas==NULL) exit(LST_NO_INI);

    if(l_size(listas)==0)  /*obtengo tamaño de lista, si es cero inserto*/
    {
        if (l_insertar(listas,NULL,elem)==TRUE) /* si es vacio inserto en el primer lugar.*/
        {
            lista_ordenada->cantidad_elementos++;
            return TRUE;
        }
        else return FALSE;
    }
    else   /*tamaño de lista diferente de 0.*/
    {

        TPosicion pos = l_primera(listas); /*obtengo primera posicion para comparar.*/

        while (comparador(l_recuperar(listas,pos),elem)<0&&(pos!=lo_ultima(lista_ordenada)))  /*debo parar*/
        {
            pos = l_siguiente(listas,pos);
        }
        if (pos==l_ultima(listas))  /*Si es la ultima debo compararla con ella, ojo,caso especial*/
        {
            if (comparador(l_recuperar(listas,pos),elem)>0)  /*inserto antes de la ultima encontrada*/
            {
                if  (l_insertar(listas,pos,elem)==TRUE)   /*retorna 1 si sale bien*/
                {
                    lista_ordenada->cantidad_elementos++;
                    return TRUE;
                }
                else return FALSE;

            }
            else    /*ultima celda*/
            {
                TCelda ult = malloc(sizeof(struct celda));
                ult->elemento=elem;
                pos->proxima_celda=ult;
                ult->proxima_celda=NULL;
                lista_ordenada->cantidad_elementos++;
                return TRUE;
            }
        }
        else    /*no es la ultima posicion*/
        {
            /*inserto antes de la ultima encontrada*/
            if  (l_insertar(listas,pos,elem)==TRUE)
            {
                lista_ordenada->cantidad_elementos++;
                return TRUE; /*retorna 1 si sale bien*/
            }
            else return FALSE;
        }
    }

    return FALSE; /*fracaso*/
}

/**
* Elimina el elemento de una posición de la lista. Reacomoda la lista adecuadamente al eliminar en posiciones intermedias.
* @param lista lista de donde se eliminará
* @param pos posición de la lista
* @return verdadero si procede con éxito, falso en caso contrario.
*/
int lo_eliminar(TListaOrdenada lista, TPosicion pos)
{
    if(lista!=NULL&&pos!=NULL&&lista->cantidad_elementos>0)
    {
        TLista l = lista->lista;
        int resultado = l_eliminar(l,pos);
        lista->cantidad_elementos--;
        if (resultado)
            return TRUE;
        else return FALSE;
    }
    else
        exit(LST_NO_INI);
}

/**
* Retorna la cantidad de elementos de la lista.
* @param lista lista a consultar cantidad de elementos
* @return cantidad de elementos de la lista
*/
int lo_size(TListaOrdenada lista)
{
    if (lista==NULL)exit(LST_NO_INI);
    return lista->cantidad_elementos;
}

/**
* Retorna la primer posicion de la lista.
* @param lista lista a examinar
* @return La primera posición de una lista.
*/
TPosicion lo_primera(TListaOrdenada lista_ordenada)
{
    if (lista_ordenada==NULL)exit(LST_NO_INI);
    return l_primera(lista_ordenada->lista);
}

/**
* Retorna la última posición de la lista.
* @param lista lista a examinar
* @return la última posición de una lista
*/
TPosicion lo_ultima(TListaOrdenada lista_ordenada)
{
    if (lista_ordenada==NULL)exit(LST_NO_INI);
    return l_ultima(lista_ordenada->lista);
}

/**
* Retorna la posición siguiente a otra en la lista
* @param lista lista a examinar
* @param pos posición de lista
* @return la posición siguiente a pos en la lista.
*/
TPosicion lo_siguiente(TListaOrdenada lista, TPosicion pos)
{
    if (lista==NULL)exit(LST_NO_INI);
    return l_siguiente(lista->lista,pos);
}
