#include <bits/stdc++.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
using namespace std;

typedef struct processo Processo;
struct processo {
    string nome;
    int start;              // t0
    int dt;                 // Tempo que o processo deve rodar
    int deadline;           // Tempo final suposto do processo
};

#define nProcessos 100
#define randStart 400
#define dtRand 20
#define deadlineRand 40
#define deadlineFixo 20

Processo processos[nProcessos];

void sort () {
    Processo aux;

    for (int i = 0; i < nProcessos; i ++) {
        for (int j = i + 1; j < nProcessos; j ++) {
            if (processos[i].start > processos[j].start) {
                aux = processos[i];
                processos[i] = processos[j];
                processos[j] = aux;
            }
        }
    }
}

int main () {
    srand (time(0));

    for (int i = 0; i < nProcessos; i ++) {
        processos[i].start = rand() % randStart;
        processos[i].dt = ( rand() % dtRand ) + 1;
        processos[i].deadline = processos[i].dt + processos[i].start + deadlineFixo + rand() % deadlineRand;
    }

    sort ();

    for (int i = 0; i < nProcessos; i ++)
        processos[i].nome = 'p' + to_string (i);

    ofstream file;
    file.open ("t.txt");

    for (int i = 0; i < nProcessos; i ++)
        file << processos[i].nome << " " << processos[i].start << " " << processos[i].dt << " " << processos[i].deadline << endl;

    file.close ();

    return 0;
}