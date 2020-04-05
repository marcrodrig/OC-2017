#include <stdlib.h>
#include "trie.h"
#include <string.h>
#include "constantes.h"
/**
* Retorna un nuevo trie vacío, esto es, con nodo raíz que mantiene
* rótulo nulo y contador en cero.
* @return estructura de trie vacía
*/
TTrie crear_trie()
{
    TTrie trie = (TTrie)malloc(sizeof(struct trie));
    TNodo nodo_raiz = (TNodo)malloc(sizeof(struct nodo));
    int el_nulo = ELE_NULO;
    nodo_raiz->rotulo = el_nulo;
    nodo_raiz->contador=0;
    nodo_raiz->hijos=NULL;
    trie->raiz = nodo_raiz;
    trie->cantidad_elementos = 0;
    return trie;
}

int f(void* x, void* y)
{
    int resultado = strcmp (x,y);
    if(resultado!=0)
    {
        if(resultado<0)
            resultado = -1;
        else
            resultado = 1;
    }
    return resultado;
}

/**
* Inserta un string en el trie, inicializando el valor de contador asociado en uno.
* En caso de que el string ya se encuentre representado en el trie, aumenta el valor
* del contador asociado a dicho string en una unidad.
* @param tr trie donde se insertará un string
* @param str string a insertar
* @return verdadero si la inserción fue exitosa, falso en caso de que el string ya perteneciera al trie.
*/
int tr_insertar(TTrie tr, char* str)
{
    if (tr==NULL) exit(TRI_NO_INI);
    int resultado;
    int nivel = 0;
    int length = strlen(str);
    TNodo nuevo_padre = tr->raiz;
    if(tr->cantidad_elementos==0) /*trie vacío*/
    {
        for(nivel = 0; nivel < length; nivel++)
        {
            if(nivel!=length)
            {
                nuevo_padre->hijos = crear_lista_ordenada(f);
            } /*creo hijos de nuevo_padre.*/
            TNodo nodo_nuevo = (TNodo)malloc(sizeof(struct nodo)); /*creo nodo y reservo memoria*/
            nodo_nuevo->padre = nuevo_padre; /*le asigno a nodo_nuevo el padre(la raiz)*/
            nodo_nuevo->contador = 0; /*le asigno el valor a contador*/
            nodo_nuevo->rotulo = str[nivel];
            if(nivel!=length)
            {
                lo_insertar(nuevo_padre->hijos,nodo_nuevo);
            }
            nuevo_padre = nodo_nuevo;
        }
        nuevo_padre->hijos=NULL;
        tr->cantidad_elementos = 1; /*aumento la cantidad de elementos del trie*/
        nuevo_padre->contador++; /*icremento en 1 la cantidad de la palabra ingresada*/
        resultado = 1; /*insertó correctamente*/
    }
    else
    {
        /* el trie no es vacio */
        int seguir = 1;
        TListaOrdenada hijos;
        while (seguir == 1 && nivel < length)
        {
            hijos = nuevo_padre->hijos;
            TPosicion nodo;

            if (hijos==NULL) seguir=0;
            else nodo = lo_primera(hijos);

            int iteraciones=0;
            int cant_maxima_iteraciones = lo_size(hijos);
            TNodo nodo_de_hijos ;
            while (iteraciones<cant_maxima_iteraciones && seguir==1)
            {
                nodo_de_hijos = (TNodo)nodo->elemento;
                if(nodo_de_hijos->rotulo == str[nivel])
                {
                    nuevo_padre = nodo_de_hijos;
                    if (nuevo_padre->hijos==NULL)seguir=0;
                    else   hijos=nuevo_padre->hijos;
                }
                else
                {
                    if (nodo==lo_ultima(hijos))
                    {
                        seguir=0;
                        nivel--;
                    }
                    else
                        nodo = lo_siguiente(hijos,nodo);
                }
                iteraciones++;
            }
            nivel++;
        }
        if(nivel==length )
        {
            /*incremento en 1 la cantidad de la palabra ingresada, la palabra ya está en el trie*/
            nuevo_padre->contador++;
            tr->cantidad_elementos++;
            resultado=0;
        }
        else
        {
            /* nivel<length */
            TNodo nodo_nuevo = (TNodo)malloc(sizeof(struct nodo));
            nodo_nuevo->padre = nuevo_padre;
            nodo_nuevo->contador = 0;
            nodo_nuevo->rotulo = str[nivel];

            if (nuevo_padre->hijos==NULL)
            {
                nuevo_padre->hijos=crear_lista_ordenada(f);
            }
            lo_insertar(nuevo_padre->hijos,nodo_nuevo);
            nuevo_padre = nodo_nuevo;
            int nivel_actualizado;
            for(nivel_actualizado = ++nivel ; nivel_actualizado < length; nivel_actualizado++)
            {
                nuevo_padre->hijos = crear_lista_ordenada(f);
                TNodo nodo_nuevo = (TNodo)malloc(sizeof(struct nodo));
                nodo_nuevo->padre = nuevo_padre;
                nodo_nuevo->contador = 0;
                nodo_nuevo->rotulo = str[nivel_actualizado];
                nodo_nuevo->hijos=crear_lista_ordenada(f);
                lo_insertar(nuevo_padre->hijos,nodo_nuevo);
                nuevo_padre = nodo_nuevo;
            }
            nuevo_padre->hijos=NULL;
            tr->cantidad_elementos ++;
            nuevo_padre->contador++;
            resultado = 1;
        }
    }
    if (resultado)
        return TRUE;
    else return FALSE;
}

/**
* Chequea si un string pertenece a un trie.
* @param tr trie a consultar
* @param str string buscado
* @return verdadero si el string pertenece al trie, falso en caso contrario.
*/
int tr_pertenece(TTrie tr, char* str)
{
    if (tr==NULL) exit(TRI_NO_INI);
    int resultado;
    int nivel = 0;
    int length = strlen(str);
    TNodo nuevo_padre = tr->raiz;
    int seguir = 1;
    TListaOrdenada hijos;
    while (seguir == 1 && nivel < length)
    {
        hijos = nuevo_padre->hijos;
        TPosicion nodo = lo_primera(hijos);

        int iteraciones=0;
        int cant_maxima_iteraciones = lo_size(hijos);
        TNodo nodo_de_hijos ;
        while (iteraciones<cant_maxima_iteraciones && seguir==1)
        {
            nodo_de_hijos = (TNodo)nodo->elemento;
            if(nodo_de_hijos->rotulo == str[nivel])
            {
                nuevo_padre = nodo_de_hijos;
                hijos=nuevo_padre->hijos;
            }
            else
            {
                if (nodo==lo_ultima(hijos))
                {
                    seguir=0;
                    nivel--;
                }
                else
                    nodo = lo_siguiente(hijos,nodo);
            }
            iteraciones++;
        }
        nivel++;
    }
    if(seguir==0 || nuevo_padre->contador==0)
        resultado=0;
    else
        resultado=1;
     if (resultado)
        return TRUE;
    else return FALSE;
}

/**
* Recupera el entero asociado a un string en un trie.
* @param tr trie a consultar
* @param str string buscado
* @return el entero asociado al string, dentro del trie.
* Si el string no pertenece al trie, retorna STR_NO_PER.
*/
int tr_recuperar(TTrie tr, char* str)
{
    if (tr==NULL) exit(TRI_NO_INI);
    int resultado;
    int nivel = 0;
    int length = strlen(str);
    TNodo nuevo_padre = tr->raiz;
    int seguir = 1;
    TListaOrdenada hijos;
    while (seguir == 1 && nivel < length)
    {
        hijos = nuevo_padre->hijos;
        if(hijos!=NULL)
        {
            TPosicion nodo = lo_primera(hijos);
            int iteraciones=0;
            int cant_maxima_iteraciones = lo_size(hijos);
            TNodo nodo_de_hijos ;
            while (iteraciones<cant_maxima_iteraciones && seguir==1)
            {
                nodo_de_hijos = (TNodo)nodo->elemento;
                if(nodo_de_hijos->rotulo == str[nivel])
                {
                    nuevo_padre = nodo_de_hijos;
                    hijos=nuevo_padre->hijos;
                }
                else
                {
                    if (nodo==lo_ultima(hijos))
                    {
                        seguir=0;
                        nivel--;
                    }
                    else
                        nodo = lo_siguiente(hijos,nodo);
                }
                iteraciones++;
            }
            nivel++;
        }
        else seguir = 0;
    }
    if(seguir==0 || nuevo_padre->contador==0)
        resultado=STR_NO_PER;
    else
        resultado=nuevo_padre->contador;
    return resultado;
}

/**
* Retorna la cantidad de palabras alamacenadas en el trie.
* @param tr trie a consultar cantidad de palabras
* @return cantidad de palabras del trie
*/
int tr_size(TTrie tr)
{
    if (tr==NULL) exit(TRI_NO_INI);
    return tr->cantidad_elementos;
}

/**
* Elimina un string dentro del trie, liberando la memoria utilizada.
* @param tr trie donde se eliminará
* @param str string a eliminar
* @return verdadero en caso de operación exitosa, y falso en caso contrario.
*/
int tr_eliminar(TTrie tr, char* str)
{
    if (tr==NULL)exit(TRI_NO_INI);
    int resultado;
    int nivel=0;
    int length = strlen(str);
    TNodo nuevo_padre = tr->raiz;
    int seguir=1;
    TListaOrdenada hijos;
    TPosicion nodo;
    while (seguir==1 && nivel<length)
    {
        hijos = nuevo_padre->hijos;
        nodo = lo_primera(hijos);
        int iteraciones = 0;
        int cant_maximia_iteracciones = lo_size(hijos);
        TNodo nodo_de_hijos;
        while (iteraciones<cant_maximia_iteracciones &&seguir==1)
        {
            nodo_de_hijos = (TNodo)nodo->elemento;
            if (nodo_de_hijos->rotulo==str[nivel])
            {
                nuevo_padre=nodo_de_hijos;
                hijos=nuevo_padre->hijos;
            }
            else
            {
                if (nodo==lo_ultima(hijos))
                {
                    seguir=0;
                    nivel--;
                }
                else
                    nodo=lo_siguiente(hijos,nodo);
            }
            iteraciones++;
        }
        nivel++;
    }
    if (seguir==0 || nuevo_padre->contador==0)
        resultado=0;
    else
        resultado =1;

    if (resultado==1)
        /* está la palabra, la elimino.*/
    {
        TListaOrdenada children = nuevo_padre->hijos; /*hijos del nodo a eliminar*/
        int esultima=1;

        TNodo eliminar = nuevo_padre; /*renombro nodo a eliminar*/

        nuevo_padre= eliminar->padre;

        TListaOrdenada hermanos = nuevo_padre->hijos;

        /*si nodo a eliminar no tiene hijos lo elimino de la lista de hijos del padre (hermanos)*/

        while (((children==NULL)||(children->cantidad_elementos==0))&&(eliminar->contador==0))
        {
            /*busco en hermanos la celda a borrar.*/
            TPosicion pos = lo_primera(hermanos);
            while(pos->elemento!=eliminar)
            {
                pos = lo_siguiente(hermanos,pos);
            }
            lo_eliminar(hermanos,pos);
            esultima=0;
            /*aca estoy en el padre del recien borrado, tengo que ver si tiene hijos y si no tiene borro, o sea repetir*/
            children= nuevo_padre->hijos;
            TNodo eliminar = nuevo_padre;


            if (nuevo_padre==tr->raiz)
            {
                /*printf("es raiz \n");*/
                break;
            }
            nuevo_padre = eliminar->padre;
            resultado=1;
        }

        if (children->cantidad_elementos!=0)         /*si nodo tiene hijos, y es la ultima letra solo cambio contador a 0.*/
            /* si no es ultima no hago nada. ya que*/
            /*No puedo borrar ya que esa letra pertenece a otra palabra.*/
        {
            if(esultima)
            {
                /*printf ("entro else \n");*/
                nuevo_padre->contador=0;
                resultado=1;
            }
        }
    } /*fin if(resultado==1)*/

    if (resultado) tr->cantidad_elementos--;
     if (resultado)
        return TRUE;
    else return FALSE;
}/*end tr_eliminar*/
