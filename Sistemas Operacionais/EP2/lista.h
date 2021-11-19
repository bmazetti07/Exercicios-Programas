#ifndef LISTA_H
#define LISTA_H

// Células da lista
typedef struct celLista CelLista;
struct celLista {
    int id;
    CelLista * prox;
};

// Insere elementos no início da lista
CelLista * insert (CelLista * lista, int id);

// Remove um elemento da lista
CelLista * removeElem (CelLista * lista, int id);

// Dá free na lista
void freeLista (CelLista * lista);

// Imprime as posições dos ciclistas na pista
void printLista (CelLista * lista, int n);

// Devolve o último colocado daquela volta (primeiro elemento da lista)
int lastID (CelLista * lista);

// Devolve o primeiro colocado que não quebrou daquela volta (último elemento da lista)
int firstID (CelLista * lista, int **quebrados);

// Conta o número de ciclistas que completaram aquela volta
int contCiclistas (CelLista * lista);

//Imprime a lista
void imprime (CelLista * lista);


#endif