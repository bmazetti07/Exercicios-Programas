#include <stdlib.h>
#include <stdio.h>

#include "lista.h"

CelLista * insert (CelLista * lista, int id) {
    CelLista * new;
    new = malloc (sizeof (CelLista));

    new -> id = id;
    new -> prox = NULL;

    if (lista != NULL)
        new -> prox = lista;
    lista = new;

    return lista;
}

CelLista * removeElem (CelLista * lista, int id) {
    CelLista *p = lista, *q = NULL;

    if (p != NULL) {
        if (p -> id == id) {
            lista = p -> prox;
            free (p);
        }
        
        else {
            for (q = p -> prox; q != NULL && q -> id != id; q = p -> prox) 
                p = q;

            if (q != NULL) {
                p -> prox = q -> prox;
                q -> prox = NULL;
                free (q);
            }
        }
    }
    
    return lista;
}

void freeLista (CelLista * lista) {
    CelLista *p, *q;

    for (p = lista; p != NULL; p = q) {
        q = p -> prox;
        free (p);
    }
}

void printLista (CelLista * lista, int n) {
    if (lista != NULL) {
        printLista (lista -> prox, n - 1);

        printf ("%3dº - %d\n", n, lista -> id);
    }
}

int lastID (CelLista * lista) {
    if (lista == NULL)
        return -1;
    
    return (lista -> id);
}

int firstID (CelLista * lista, int **quebrados) {
    CelLista * aux = lista;
    CelLista * first = NULL;

    while (aux != NULL) {
        if (quebrados[aux -> id][0] == 0)
            first = aux;
        aux = aux -> prox;
    }

    if (first != NULL)
        return (first -> id);
    return -1;
}

int contCiclistas (CelLista * lista) {
    CelLista * aux;
    int n = 0;

    for (aux = lista; aux != NULL; aux = aux -> prox)
        n ++;

    return n;
}

void imprime (CelLista * lista) {
    CelLista * aux;
    printf ("Lista: ");
    for (aux = lista; aux != NULL; aux = aux -> prox)
        printf ("%3d ", aux -> id);
    printf ("\n");
}