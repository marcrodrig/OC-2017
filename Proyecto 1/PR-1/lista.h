/**TDA Lista simplemente enlazada sin centinelas, con elementos punteros genéricos**/

typedef void * TElemento;

typedef struct celda
{
    TElemento elemento;
    struct celda * proxima_celda;
} * TCelda;

typedef struct lista
{
    unsigned int cantidad_elementos;
    TCelda primer_celda;
} * TLista;

typedef struct celda * TPosicion;

/** Servicios del TDA Lista **/

/**
* Crea y retorna una lista vacía.
* @return Estructura de lista vacía
*/
TLista crear_lista();

/**
* Agrega el elemento elem en la posicíon anterior a pos, dentro de la lista. Si pos es POS_NULA, inserta el
* elemento en la primer posicíon de la lista.
* @param lista lista donde se insertará un elemento
* @param pos posición siguiente donde se insertará el elemento
* @param elem elemento que se insertará
* @return verdadero si procede con ́exito, falso en caso contrario.
*/
int l_insertar(TLista lista, TPosicion pos, TElemento elem);

/**
* Elimina el elemento en la posicíon pos
* Reacomoda la lista adecuadamente.
* @param lista lista se eliminará un elemento
* @param pos posición con elemento a eliminar
* @return Retorna verdadero si procede con ́exito, falso en caso
* contrario. Si la posición no es válida retornar LST_POS_INV.
*/
int l_eliminar(TLista lista, TPosicion pos);

/**
* Retorna la primer posicion de la lista.
* @param lista lista a examinar
* @return La primera posición de una lista. Si la lista es vacía retorna POS_NULA.
*/
TPosicion l_primera(TLista lista);

/**
* Retorna la última posición de la lista.
* @param lista lista a examinar
* @return la última posición de una lista. Si la lista es vacía retorna POS_NULA.
*/
TPosicion l_ultima(TLista lista);

/**
* Retorna la posición anterior a otra en la lista
* @param lista lista a examinar
* @param pos posición de lista
* @return la posición anterior a pos en la lista. Si pos es la primer posición de la lista, retorna POS_NULA.
*/
TPosicion l_anterior(TLista lista, TPosicion pos);

/**
* Retorna la posición siguiente a otra en la lista
* @param lista lista a examinar
* @param pos posición de lista
* @return la posición siguiente a pos en la lista. Si pos es la última posición de la lista o POS_NULA, retorna POS_NULA.
*/
TPosicion l_siguiente(TLista lista, TPosicion pos);

/**
* Retorna el elemento correspondiente a una posición de una lista.
* @param lista lista a examinar
* @param pos posición de lista
* @return el elemento correspondiente a la posición pos. Si la posición es POS_NULA, retorna ELE_NULO.
*/
TElemento l_recuperar(TLista lista, TPosicion pos);

/**
* Retorna la cantidad de elementos de la lista.
* @param lista lista a consultar cantidad de elementos
* @return cantidad de elementos de la lista
*/
int l_size(TLista lista);
