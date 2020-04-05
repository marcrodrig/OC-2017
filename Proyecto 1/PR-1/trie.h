#include "lista_ordenada.h"
/**TDA Trie**/

typedef struct nodo
{
    char rotulo;
    unsigned int contador;
    struct nodo * padre;
    TListaOrdenada hijos;
} * TNodo;

typedef struct trie
{
    unsigned int cantidad_elementos;
    TNodo raiz;
} * TTrie;

/** Servicios del TDA Trie **/

/**
* Retorna un nuevo trie vacío, esto es, con nodo raíz que mantiene
* rótulo nulo y contador en cero.
* @return estructura de trie vacía
*/
TTrie crear_trie();

/**
* Inserta un string en el trie, inicializando el valor de contador asociado en uno.
* En caso de que el string ya se encuentre representado en el trie, aumenta el valor
* del contador asociado a dicho string en una unidad.
* @param tr trie donde se insertará un string
* @param str string a insertar
* @return verdadero si la inserción fue exitosa, falso en caso de que el string ya perteneciera al trie.
*/
int tr_insertar(TTrie tr, char* str);

/**
* Chequea si un string pertenece a un trie.
* @param tr trie a consultar
* @param str string buscado
* @return verdadero si el string pertenece al trie, falso en caso contrario.
*/
int tr_pertenece(TTrie tr, char* str);

/**
* Recupera el entero asociado a un string en un trie.
* @param tr trie a consultar
* @param str string buscado
* @return el entero asociado al string, dentro del trie.
* Si el string no pertenece al trie, retorna STR_NO_PER.
*/
int tr_recuperar(TTrie tr, char* str);

/**
* Retorna la cantidad de palabras alamacenadas en el trie.
* @param tr trie a consultar cantidad de palabras
* @return cantidad de palabras del trie
*/
int tr_size(TTrie tr);

/**
* Elimina un string dentro del trie, liberando la memoria utilizada.
* @param tr trie donde se eliminará
* @param str string a eliminar
* @return verdadero en caso de operación exitosa, y falso en caso contrario.
*/
int tr_eliminar(TTrie tr, char* str);
