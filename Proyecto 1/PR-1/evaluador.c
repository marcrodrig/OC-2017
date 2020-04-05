#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "constantes.h"

#define letra_minuscula_valida ((caracteres[i]>=97 && caracteres[i]<=122) || caracteres[i]==-61 || caracteres[i]==-79)
#define letra_minuscula_tilde_valida (caracteres[i]==-95 || caracteres[i]==-87 || caracteres[i]==-83 || caracteres[i]==-77 || caracteres[i]==-70)


int ingreso_nro_de_operacion()
{
    int nro_operacion;
    int nro_valido=0;
    printf("Ingrese el número de operación que desea realizar: ");
    while (!nro_valido)
    {

        if(scanf("%i",&nro_operacion) != 1 )
        {
            printf("No ingresó un número, ingrese un número de operación: ");
            /*limpio buffer*/
            while(getchar() != '\n');
        }
        else
        {
            if(nro_operacion<1 || nro_operacion>6)
            {
                printf("No ingresó un número válido, ingrese nuevamente un número de operación: ");
            }
            else
            {
                nro_valido=1;
            }
        }
    }
    return nro_operacion;
}

void armarPalabra (TTrie trie,TNodo nodo)
{
    char palabra[50];
    palabra[0]=nodo->rotulo;
    nodo=nodo->padre;
    int cantLetras=1;
    int i=0;

    while (nodo!=trie->raiz)
    {
        while (i<cantLetras)
        {
            palabra[cantLetras-i]=palabra[cantLetras-1-i];
            i++;
        }
        palabra[0]=nodo->rotulo;
        cantLetras++;
        i=0;
        nodo=nodo->padre;
    }
    palabra[cantLetras]=0;

    printf("%s \n", palabra); /*imprimo palabra*/
}

void recorroParaMostrar(TTrie trie,TNodo raiz)
{
    int cant_hijos=0;
    if (raiz->contador>0)
        armarPalabra(trie,raiz);

    if (raiz->hijos!=NULL)
    {
        TListaOrdenada lista_hijos = raiz->hijos;
        cant_hijos = lo_size(lista_hijos);
        int iteraciones = 0;
        if (cant_hijos>0)
        {
            TPosicion pos = lo_primera(lista_hijos);

            while(iteraciones<cant_hijos)
            {
                raiz = pos->elemento;
                recorroParaMostrar(trie,raiz);
                if (pos!=lo_ultima(lista_hijos))
                    pos = lo_siguiente(lista_hijos,pos);
                iteraciones++;
            }
        }
    }

}

void mostrarPalabras (TTrie trie)
{
    /* Debo recorrer el arbol y cuendo encuentro un nodo con cantidad>0,
    cuando lo encuentro armo la palabra subiendo hasta la raiz. */
    TNodo n = trie->raiz;
    printf("# Operación 1 : Mostrar Palabras #\n");
    recorroParaMostrar(trie,n);
}

void consultar(TTrie trie)
{
    printf("# Operación 2 : Consultar #\n");
    char* palabra = malloc(40);
    printf("Ingrese una palabra para chequear si está en el archivo: ");
    scanf("%s",palabra);
    int resultado = tr_recuperar(trie, palabra);
    if (resultado == STR_NO_PER)
        printf("La palabra ingresada no pertenece al archivo\n");
    else
        printf("La palabra ingresada está en el archivo, la cantidad de veces que aparece es %i\n",resultado);
}


int contar_prefijos_preorden(TNodo raiz)
{
    int cant_prefijos = 0;
    int cant_hijos=0;
    if (raiz->hijos!=NULL)
    {
        TListaOrdenada lista_hijos = raiz->hijos;
        cant_hijos = lo_size(lista_hijos);
        int iteraciones = 0;
        if (cant_hijos>0)
        {
            TPosicion pos = lo_primera(lista_hijos);

            while(iteraciones<cant_hijos)
            {
                raiz = pos->elemento;
                if (raiz->contador>0)
                    cant_prefijos++;
                cant_prefijos = cant_prefijos + contar_prefijos_preorden(raiz);
                if (pos!=lo_ultima(lista_hijos))
                    pos = lo_siguiente(lista_hijos,pos);
                iteraciones++;
            }
        }
    }
    return cant_prefijos;
}

void comienza_con(TTrie trie)
{
    printf("# Operación 3 : Comienza con #\n");
    char* caracteres=malloc(sizeof(char*));
    printf("Ingrese una letra para chequear cuántas palabras comienzan con esa letra: ");
    scanf("%s",caracteres);
    int longitud = strlen(caracteres);
    int resultado = 1;
    int i=0;
    if(strlen(caracteres)<=2)
    {
        int mayores = 0;
        while (i<longitud && resultado==1)
        {
            resultado = letra_minuscula_valida || letra_minuscula_tilde_valida;
            if(caracteres[i]>0)
                mayores++;
            i++;
        }
        if(mayores>1)
            resultado=0;
        if(resultado==1) /*letra válida, busco esa letra dentro del trie*/
        {
            int nivel = 0;
            int length = strlen(caracteres);
            TNodo nuevo_padre = trie->raiz;
            int seguir = 1;
            TListaOrdenada hijos=NULL;
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
                    if(nodo_de_hijos->rotulo == caracteres[nivel])
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
            if(seguir==1)
            {
                //si tam la lista de hijos es 0, res es 0 sino me posiciono en primero de lista y recorro en preorden
                if(hijos!=NULL && lo_size(hijos)!=0)
                {
                    int resultado = nuevo_padre->contador;
                    resultado = resultado + contar_prefijos_preorden(nuevo_padre);
                    printf("La cantidad de palabras que comienzan con la letra '%s' es %i.\n",caracteres,resultado);

                }
                else
                    /*la unica palabra con esa letra, es la letra misma*/
                    printf("La cantidad de palabras que comienzan con la letra '%s' es 1.\n",caracteres);
            }
            else
            {
                /*letra válida pero ninguna palabra comienza con ella*/
                printf("La cantidad de palabras que comienzan con la letra '%s' es 0.\n",caracteres);
            }
        }
        else printf("No ingresó una letra válida.\n");
    }
    else
    {
        printf("No ingresó una letra válida.\n");
    }
}


void es_prefijo(TTrie trie)
{
    printf("# Operación 4 : Es prefijo #\n");
    char* caracteres=malloc(sizeof(char*));
    printf("Ingrese una palabra para chequear si es prefijo de otras: ");
    scanf("%s",caracteres);

    /*busco esa palabra dentro del trie*/
    int nivel = 0;
    int length = strlen(caracteres);
    TNodo nuevo_padre = trie->raiz;
    int seguir = 1;
    TListaOrdenada hijos=NULL;
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
            if(nodo_de_hijos->rotulo == caracteres[nivel])
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
    if(seguir==1)
    {
        /*estoy posicionado en la ultima letra de la palabra*/
        if(hijos!=NULL && lo_size(hijos)!=0)
        {
            printf("La palabra '%s' es prefijo de otras.\n",caracteres);
        }
        else
            /*la {unica palabra con esa letra, es la letra misma*/
            printf("La palabra '%s' está pero no es prefijo de otras.\n",caracteres);
    }
    else
        printf("La palabra '%s' no es prefijo de ninguna.\n",caracteres);
}


void porcentaje_prefijo(TTrie trie)
{
    printf("# Operación 5 : Es prefijo #\n");
    char* caracteres=malloc(sizeof(char*));
    printf("Ingrese una palabra para chequear si es prefijo de otras y en que porcentaje de palabras aparece: ");
    scanf("%s",caracteres);
    /*busco esa palabra dentro del trie*/
    int nivel = 0;
    int length = strlen(caracteres);
    TNodo nuevo_padre = trie->raiz;
    int seguir = 1;
    TListaOrdenada hijos=NULL;
    TNodo contarDesdeEste;

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
            if(nodo_de_hijos->rotulo == caracteres[nivel])
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
        contarDesdeEste = nuevo_padre;
    }
    if(seguir==1)
    {
        /*estoy posicionado en la ultima letra de la palabra (ṕrefijo)*/
        if(hijos!=NULL && lo_size(hijos)!=0)
        {
            /*Tengo que recorrer desde la letra donde estoy hacia abajo y
            ver cuantos nodos con contador>0 hay e ir sumando */
            int totalPalabras=0;

            totalPalabras = contar_prefijos_preorden(contarDesdeEste);
            float porcentaje= totalPalabras*100/tr_size(trie);
            printf("La palabra '%s' es prefijo de otras.\n",caracteres);
            printf("Y aparece en un %.2f por ciento de palabras \n",porcentaje);
        }
        else
            /*la unica palabra con esa letra, es la letra misma*/
            printf("La palabra '%s' está pero no es prefijo de otras.\n",caracteres);
    }
    else
        printf("La palabra '%s' no es prefijo de ninguna.\n",caracteres);
}


void recorroYborro(TTrie trie,TNodo raiz)
{

    if (raiz!=NULL||trie!=NULL)
    {
    int cant_hijos=0;
    TListaOrdenada lista_hijos = raiz->hijos;
    TPosicion pos = NULL;
    int iteraciones = 0;
    int borro = 0;
    if ((lista_hijos!=NULL)&&(lista_hijos->cantidad_elementos>0))
    {
        cant_hijos = lo_size(lista_hijos);
        iteraciones = 0;
        pos = lo_primera(lista_hijos);

        while(iteraciones<cant_hijos)
        {
            raiz = pos->elemento;
            recorroYborro(trie,raiz);
            if (pos!=lo_ultima(lista_hijos))
                pos = lo_siguiente(lista_hijos,pos);
            iteraciones++;
        }
    }
    else
    {
        /*No tiene hijos el nodo raiz.*/
        /* Ya que no tiene hijos, lo elimino de la lista del padre. y libero memoria*/
        if(raiz!=trie->raiz)
        {
            lista_hijos =raiz->padre->hijos;
            pos = lo_primera(lista_hijos);
            iteraciones = 0;
            while(pos->elemento!=raiz && iteraciones<lo_size(lista_hijos))
            {
                TNodo nodo = pos->elemento;
                if (nodo!= raiz)
                    pos = lo_siguiente(lista_hijos,pos);
                iteraciones++;
            }
            if (pos!=NULL)
            {
                lo_eliminar(lista_hijos,pos);
                borro=1;
                free(raiz->hijos);
                free(raiz);
            }

        }
        else
        { /*es raiz de trie.*/
          free(raiz->hijos);
          free(raiz);
        }
    }
    if (!borro&&lista_hijos!=NULL&&pos!=NULL&&lista_hijos->cantidad_elementos>0)
    {
        lo_eliminar(lista_hijos,pos);
        free(raiz->hijos);
        free(raiz);
    }
    }
}

void salir(TTrie trie)
{
    /* Tengo que recorrer nodo por nodo e ir eliminando nodo a nodo, de hojas hacia raiz */
    recorroYborro(trie,trie->raiz);
    free(trie);
    printf("Operación realizada con éxito\n");
    exit(EXIT_SUCCESS);
}

void operaciones(TTrie trie)
{
    printf("¿Qué operación desea realizar ahora? ");
    int nro_operacion = ingreso_nro_de_operacion();
    switch(nro_operacion)
    {
    case 1:
        mostrarPalabras(trie);
        operaciones(trie);
        break;
    case 2:
        consultar(trie);
        operaciones(trie);
        break;
    case 3:
        comienza_con(trie);
        operaciones(trie);
        break;
    case 4:
        es_prefijo(trie);
        operaciones(trie);
        break;
    case 5:
        porcentaje_prefijo(trie);
        operaciones(trie);
        break;
    case 6:
        salir(trie);
        break;
    }
}

void menu_operaciones(FILE* archivo_texto)
{
    char* caracteres; /*para leer cada renglon*/

    caracteres=(char*)malloc(100*sizeof(char));

    TTrie trie =  crear_trie();
    char* palabra;
    palabra = (char*)malloc(40*sizeof(char));
    while (feof(archivo_texto)==0)
    {
        fgets(caracteres,80,archivo_texto);

        int i=0;
        int longitud=strlen(caracteres);
        int j=0;
        for (i=0; i<=longitud; i++) /*leo todo el renglon en busca de caracteres no validos*/
        {
            if (letra_minuscula_valida || letra_minuscula_tilde_valida)  /*si son minusculas armo palabra*/
            {
                /*estoy dentro de una palabra.*/
                palabra[j]=caracteres[i];
                j++;
            }
            else   /*si no es una letra valida puedo haber armado una palabra o estar pasando caracteres no validos*/
            {
                int l = strlen(palabra);
                if (l>0)   /* si longitud de palabra es mayor a 0 arme una cadena*/
                {
                    palabra[l]='\0';
                    if( tr_insertar(trie,palabra) == 1)
                        printf("Insertó la palabra '%s' o las letras restantes si el comienzo de la palabra ya estaba en el trie\n",palabra);

                    else /*=0*/
                        printf("La palabra '%s' ya estaba en el trie o está en el comienzo de otra palabra ya insertada, incrementé la cantidad en 1\n",palabra);
                    int h=0;
                    while (h<40)  /*borro palabra usada*/
                    {
                        palabra[h]=0;
                        h++;
                    }
                    j=0;
                }/*fin if*/
            }/*fin else*/
        }
    }
    free(caracteres);
    free(palabra);
    fclose(archivo_texto); //cierro archivo, ya no lo uso

    if(trie->cantidad_elementos > 0)
    {
        printf("Menú de operaciones:\n");
        printf("1- Mostras palabras: permite visualizar el listado de todas las palabras juntos con la cantidad de apariciones de la misma\n");
        printf("2- Consultar: permite determinar si una palabra ingresada pertenece o no al archivo, y en consecuencia, cuántas veces ésta se repite en el archivo\n");
        printf("3- Comienza con: permite consultar cuántas palabras comienzan con una letra dada\n");
        printf("4- Es prefijo: permite consultar si una palabra ingresada es prefijo de otras en el archivo\n");
        printf("5- Porcentaje prefijo: dado un prefijo, indica el pocentaje de palabras en el archivo que comienzan con él\n");
        printf("6- Salir: permite salir del programa\n");
        operaciones(trie);
    }
    else
        printf("El archivo no contiene palabras válidas.\n");
}


int main(int argc, char *argv[])
{
    printf("##### EVALUADOR DE ARCHIVO DE TEXTO#####\n");
    if(argc==2)
    {
        char* nombre_archivo_texto = argv[1];
        FILE* archivo_texto;

        if((archivo_texto= fopen(nombre_archivo_texto,"r"))==NULL)
        {
            /*Abro el archivo en modo lectura*/
            printf ("El archivo ingresado no es correcto.\n");
            return 0;
        }
        else
        {
            menu_operaciones(archivo_texto);
        }
    }
    else
    {
        printf ("Error en cantidade de argumentos\n");
        return 1;
    }
    return 0; /*?*/
}
