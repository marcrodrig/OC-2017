#include <stdlib.h>
#include "constantes.h"
#include "lista.h"

/**
* Crea y retorna una lista vacía.
* @return Estructura de lista vacía
*/
TLista crear_lista()
{
    TLista l = malloc(sizeof(struct lista));
    l->primer_celda=NULL;
    l->cantidad_elementos = 0;
    return l;
}

/**
* Agrega el elemento elem en la posicíon anterior a pos, dentro de la lista. Si pos es POS_NULA, inserta el
* elemento en la primer posicíon de la lista.
* @param lista lista donde se insertará un elemento
* @param pos posición siguiente donde se insertará el elemento
* @param elem elemento que se insertará
* @return verdadero si procede con ́exito, falso en caso contrario.
*/
int l_insertar(TLista lista, TPosicion pos, TElemento elem)
{
    if (lista==NULL)
        exit (LST_NO_INI);
    if(pos==POS_NULA)   /*inserta al principio*/
    {
        TCelda t=malloc(sizeof(struct celda));
        t->elemento=elem;
        if(l_size(lista)==0)
            t->proxima_celda=NULL;
        else
            t->proxima_celda=lista->primer_celda;
        lista->primer_celda=t;
        lista->cantidad_elementos++;
        return TRUE;
    }

    else
    {
        TPosicion anterior_pos = l_anterior(lista,pos);
       // TPosicion siguiente_pos = l_siguiente(lista,pos);
        TPosicion primera_pos = l_primera(lista);
        TPosicion ultima_pos = l_ultima(lista);
        TCelda t=malloc(sizeof(struct celda));
        t->elemento=elem;
        t->proxima_celda=pos;
        if(pos==primera_pos ) /*hay un solo elemento, inserto al principio*/
            lista->primer_celda = t;
        else
        {
            TPosicion resultado = primera_pos;
            while(resultado!=ultima_pos && resultado!=pos)
                resultado=l_siguiente(lista,resultado);
            if(resultado==pos || resultado==ultima_pos)
                anterior_pos->proxima_celda=t;
            else /*pos no está en la lista*/
                return FALSE;
        }
        lista->cantidad_elementos++;
        return TRUE;
    }
}

/**
* Elimina el elemento en la posicíon pos
* Reacomoda la lista adecuadamente.
* @param lista lista se eliminará un elemento
* @param pos posición con elemento a eliminar
* @return Retorna verdadero si procede con ́exito, falso en caso
* contrario. Si la posición no es válida retornar LST_POS_INV.
*/
int l_eliminar(TLista lista, TPosicion pos)
{
    if (lista==NULL)
        exit (LST_NO_INI);
    if(l_size(lista)==0)
        return FALSE;  
    if (pos==NULL) return LST_POS_INV;
    else
    {
        TPosicion anterior_pos = l_anterior(lista,pos);
        TPosicion siguiente_pos = l_siguiente(lista,pos);
        TPosicion primera_pos = l_primera(lista);
        TPosicion ultima_pos = l_ultima(lista);
        if(pos==primera_pos)   /*elimino primero*/
        {
            lista->primer_celda=siguiente_pos;
        }
        else if(pos==ultima_pos)
            anterior_pos->proxima_celda=NULL;
        else
        {
            TPosicion resultado = primera_pos;
            while(resultado!=ultima_pos && resultado!=pos)
                resultado = l_siguiente(lista,resultado);
            if(resultado==pos)
                anterior_pos->proxima_celda=siguiente_pos;
            if(resultado==ultima_pos) /*pos no está en la lista*/
                return LST_POS_INV;
        }
        lista->cantidad_elementos--;
        free(pos);
        pos =NULL;
        return TRUE ;
    }
}

/**
* Retorna la primer posicion de la lista.
* @param lista lista a examinar
* @return La primera posición de una lista. Si la lista es vacía retorna POS_NULA.
*/
TPosicion l_primera(TLista lista)
{
    if (lista==NULL)
        exit (LST_NO_INI);
    if(l_size(lista)==0)
        return POS_NULA;
    else
        return lista->primer_celda;
}

/**
* Retorna la última posición de la lista.
* @param lista lista a examinar
* @return la última posición de una lista. Si la lista es vacía retorna POS_NULA.
*/
TPosicion l_ultima(TLista lista)
{
    if (lista==NULL)
        exit (LST_NO_INI);
    if(l_size(lista)==0)
        return POS_NULA;
    else
    {
        TPosicion ultima = l_primera(lista);
        while(ultima->proxima_celda!=NULL)
            ultima = l_siguiente(lista,ultima);
        return ultima;
    }
}

/**
* Retorna la posición anterior a otra en la lista
* @param lista lista a examinar
* @param pos posición de lista
* @return la posición anterior a pos en la lista. Si pos es la primer posición de la lista, retorna POS_NULA.
*/
TPosicion l_anterior(TLista lista, TPosicion pos)
{
    if (lista==NULL)
        exit (LST_NO_INI);
    if (pos==lista->primer_celda)
        return POS_NULA;
    else
    {
        TPosicion resultado = l_primera(lista);
        while(resultado->proxima_celda!=pos)
            resultado = l_siguiente(lista,resultado);
        return resultado;
    }
}

/**
* Retorna la posición siguiente a otra en la lista
* @param lista lista a examinar
* @param pos posición de lista
* @return la posición siguiente a pos en la lista. Si pos es la última posición de la lista o POS_NULA, retorna POS_NULA.
*/
TPosicion l_siguiente(TLista lista, TPosicion pos)
{
    if (lista==NULL)
        exit (LST_NO_INI);
    if(pos==POS_NULA)
        return POS_NULA;
    else
        return pos->proxima_celda;
}

/**
* Retorna el elemento correspondiente a una posición de una lista.
* @param lista lista a examinar
* @param pos posición de lista
* @return el elemento correspondiente a la posición pos. Si la posición es POS_NULA, retorna ELE_NULO.
*/
TElemento l_recuperar(TLista lista, TPosicion pos)
{
    if (lista==NULL)
        exit (LST_NO_INI);
    if (pos==POS_NULA)
        return ELE_NULO;
    else
    {
        TPosicion resultado = l_primera(lista);
        while(resultado!=pos)
            resultado = l_siguiente(lista,resultado);
        return resultado->elemento;
    }
}

/**
* Retorna la cantidad de elementos de la lista.
* @param lista lista a consultar cantidad de elementos
* @return cantidad de elementos de la lista
*/
int l_size(TLista lista)
{
    if (lista==NULL)
        exit (LST_NO_INI);

    return lista->cantidad_elementos;
}
