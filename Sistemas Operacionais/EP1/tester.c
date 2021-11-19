#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define maxProcessos 10000

typedef struct processo Processo;
struct processo {
    char nome[30];
    int start;              // t0
    int dt;                 // Tempo que o processo deve rodar
    int deadline;           // Tempo final suposto do processo
};

Processo processos[maxProcessos];
int nProc = 0;

void readFile (char * filename) {
    FILE * trace;
    trace = fopen (filename, "r");

    while (!feof (trace)) {
        if (fscanf (trace, "%s %d %d %d", processos[nProc].nome, &processos[nProc].start, &processos[nProc].dt, &processos[nProc].deadline) != 4)
            continue;
        nProc ++;
    }

    fclose (trace);
}

int main (int argc, char **argv) {
    readFile (argv[1]);

    int totalTime = processos[0].start;
    int deadlinesNoCump = 0;

    for (int i = 0; i < nProc; i ++) {
        if (totalTime >= processos[i].start)
            totalTime = totalTime + processos[i].dt;
        else
            totalTime = processos[i].start + processos[i].dt;
        
        if (totalTime > processos[i].deadline)
            deadlinesNoCump ++;
    }

    printf ("Total de deadlines não cumpridas: %d\n", deadlinesNoCump);

    return 0;
}