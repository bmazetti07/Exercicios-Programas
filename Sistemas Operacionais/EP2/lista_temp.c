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

        printf ("%dº - %d\n", n, lista -> id);
    }
}

int lastID (CelLista * lista) {
    if (lista == NULL)
        return -1;
    
    return (lista -> id);
}

int contCiclistas (CelLista * lista) {
    CelLista * aux;
    int n = 0;

    for (aux = lista; aux != NULL; aux = aux -> prox)
        n ++;

    return n;
}
