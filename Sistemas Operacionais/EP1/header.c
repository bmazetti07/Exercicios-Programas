#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "header.h"

void createFila (int size) {
    fila = malloc ((size + 3) * sizeof (Processo));
    
    tamFila = size + 3;
    iniFila = fimFila = 0; 
    nFila = 0;
}

void freeFila () {
    free (fila);
}

void resizeFila () {
    Processo * novo = malloc (5 * tamFila);

    for (int i = 0; i < nFila; i ++)
        novo[i] = fila[(iniFila + i) % tamFila];
    
    freeFila ();

    iniFila = 0;
    fimFila = nFila;

    tamFila *= 5;

    fila = novo;
}

void queue (Processo proc) {
    if ((fimFila + 1) % tamFila == iniFila)
        resizeFila ();
    
    fila[fimFila] = proc;

    fimFila = (fimFila + 1) % tamFila;

    nFila ++;
}

Processo dequeue () {
    Processo aux;

    nFila --;
    aux = fila[iniFila];
    iniFila = (iniFila + 1) % tamFila;

    return aux;
}

Processo getIniFila () {
    return (fila[iniFila]);
}

int emptyFila () {
    if (iniFila == fimFila)
        return 1;
    return 0;
}

void printFila () {
    printf ("   Fila: ");
    for (int i = iniFila; i < fimFila; i = (i + 1) % tamFila) {
        printf (" %s ", fila[i].nome);
    }
    
    if (!emptyFila ()) {
        printf("\n   Fila: ");
        for (int i = iniFila; i < fimFila; i = (i + 1) % tamFila) {
            printf (" %d ", fila[i].dt);
        }
    }
    
    printf ("\n");
}

void sortFila () {
    Processo aux;
    
    for (int i = iniFila; i < fimFila; i = (i + 1) % tamFila) {
        for (int j = i + 1; j < fimFila; j = (j + 1) % tamFila) {
            if (fila[i].dt > fila[j].dt) {
                aux = fila[i];
                fila[i] = fila[j];
                fila[j] = aux;
            }
        }
    }
}

void diminuiDt (int val) {
    fila[iniFila].dt -= val;
}