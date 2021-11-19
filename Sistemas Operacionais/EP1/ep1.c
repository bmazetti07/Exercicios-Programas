#define _GNU_SOURCE                                 // Para a função sched_getcpu();

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "header.h"

#define maxProcessos 300000 + 7                     // Indica o número máximo de processos

// VARIÁVEIS GLOBAIS DESTINADOS
Processo processos[maxProcessos];                   // Processos lidos do arquivo de trace
int tempos[500000];                                 // Número de processos existentes em cada t0
int nProc = 0;                                      // Número de processos totais
int procAtual = 0;                                  // Processo atuando no momento
int type = 0;                                       // Tipo do escalonador
int mudaContexto = 0;                               // Número de mudanças de contexto           
bool paramD = false;                                // Sinaliza se a flag "d" foi solicitada
int deadLineNotCump = 0;

// SEMÁFOROS PARA OS PROCESSOS
pthread_mutex_t sem[maxProcessos];

// LÊ O ARQUIVO DE TRACE
void readFile (char * filename) {
    FILE * trace;
    trace = fopen (filename, "r");

    while (!feof (trace)) {
        if (fscanf (trace, "%s %d %d %d", processos[nProc].nome, &processos[nProc].start, &processos[nProc].dt, &processos[nProc].deadline) != 4)
            continue;
        processos[nProc].id = nProc;
        processos[nProc].created = false;
        processos[nProc].finishedDef = false;
        processos[nProc].finishedOp = false;
        processos[nProc].tf = -1;
        processos[nProc].runCount = 0.0;
        tempos[processos[nProc].start] ++;
        nProc ++;
    }

    fclose (trace);
}

// ESCREVE NO ARQUIVO FILENAME: NOME DO PROCESSO | TF | TR
void writeFile (char * filename) {
    FILE * file;
    file = fopen (filename, "w");

    for (int i = 0; i < nProc; i ++)
        fprintf (file, "%-30s %10d %10d\n", processos[i].nome, processos[i].tf, processos[i].tf - processos[i].start);
    fprintf (file, "%d", mudaContexto);
}

// FUNÇÃO PARA CADA UM DOS PROCESSOS
void * work (void * parameters) {
    int index = *( (int *) parameters);
    int op = 0;

    if (type == 1) {
        if (paramD)
            fprintf (stderr, "Processo [%s] começou a usar a [CPU %d]\n", processos[index].nome, sched_getcpu());

        while (processos[index].finishedDef == false) {
            op ++;
            op --;
        }

        if (paramD)
            fprintf (stderr, "Processo [%s] liberou a [CPU %d]\n", processos[index].nome, sched_getcpu());

    }

    else if (type == 2 || type == 3) {
        while (processos[index].finishedDef == false && !pthread_mutex_lock (&sem[index])) {
            if (paramD)
                fprintf (stderr, "Processo [%s] começou a usar a [CPU %d]\n", processos[index].nome, sched_getcpu());
                
            processos[index].finishedOp = false;
            while (processos[index].finishedOp == false) {
                op ++;
                op --;
            }

            if (paramD)
                fprintf (stderr, "Processo [%s] liberou a [CPU %d]\n", processos[index].nome, sched_getcpu());
        }
    }
    
    return NULL;
}

// ESCALONADOR FIRST-COME FIRST-SERVED
void * fcfs (void * arguments) {
    pthread_t thread;
    Processo aux;
    int relogio = 0;
    int procFinalizados = 0;
    int id;

    createFila (nProc);
    while (procFinalizados != nProc) {
        while (procAtual < nProc && tempos[relogio]) {
            if (paramD)
                fprintf (stderr, "O processo [%s] chegou no sistema: [t0 - %d] | [dt - %d] | [deadline - %d]\n", processos[procAtual].nome, processos[procAtual].start, processos[procAtual].dt, processos[procAtual].deadline);

            queue (processos[procAtual]);
            procAtual ++;
            tempos[relogio] --;
        }

        if (!emptyFila ()) {
            aux = getIniFila ();
            id = aux.id;

            if (processos[id].created == true && (int) processos[id].runCount >= processos[id].dt) {
                processos[id].finishedDef = true;

                if (pthread_join (thread, NULL)) {
                    printf ("ERRO ao esperar o término da thread!");
                    exit (1);
                }

                pthread_detach (thread);
                procFinalizados ++;
                processos[id].tf = relogio;

                if (processos[id].tf > processos[id].deadline)
                    deadLineNotCump ++;

                if (paramD)
                    fprintf (stderr, "O processo [%s] finalizou: [tf - %d] | [tr - %d]\n", processos[id].nome, processos[id].tf, processos[id].tf - processos[id].start);

                dequeue ();
                if (procFinalizados == nProc)
                    break;

                if (!emptyFila ()) {
                    mudaContexto ++;
                    if (paramD)
                        fprintf (stderr, "Nova mudança de contexto. Total de mudanças até agora: %d\n", mudaContexto);
                    
                    aux = getIniFila ();
                    id = aux.id;
                }
            }

            if (processos[id].created == false) {
                if (pthread_create (&thread, NULL, work, (void *) &id)) {
                    printf ("ERRO ao criar a thread!");
                    exit (1);
                }

                processos[id].created = true;
            }

            usleep (1000000);
            relogio ++;
            processos[id].runCount ++;
        }

        else {
            usleep (1000000);
            relogio ++;
        }
    }

    freeFila ();
    return NULL;
}

// ESCALONADOR SHORTEST REMAINING TIME NEXT
void * srtn (void * arguments) {
    pthread_t threads[nProc];
    Processo aux, running;
    bool newEntry = false;
    bool firstExec = true;
    bool changed = true;
    int relogio = 0;
    int procFinalizados = 0;
    int id;

    createFila (nProc);
    while (procFinalizados != nProc) {
        while (procAtual < nProc && tempos[relogio]) {
            if (paramD)
                fprintf (stderr, "O processo [%s] chegou no sistema: [t0 - %d] | [dt - %d] | [deadline - %d]\n", processos[procAtual].nome, processos[procAtual].start, processos[procAtual].dt, processos[procAtual].deadline);

            queue (processos[procAtual]);
            procAtual ++;
            tempos[relogio] --;
            newEntry = true;
        }

        if (newEntry) 
            sortFila ();
        if (firstExec && newEntry) {
            running = getIniFila ();
            firstExec = false;
        }

        if (!emptyFila ()) {
            aux = getIniFila ();
            id = aux.id;

            if (newEntry == false) {
                if (strcmp (aux.nome, running.nome) == 0)
                    changed = false;
                else
                    changed = true;
            }

            else if (newEntry == true && strcmp (aux.nome, running.nome) != 0) {
                processos[running.id].finishedOp = true;

                if (!processos[running.id].finishedDef && !firstExec) {
                    mudaContexto ++;
                    if (paramD)
                        fprintf (stderr, "Nova mudança de contexto. Total de mudanças até agora: %d\n", mudaContexto);
                }
                running = aux;
                changed = true;
            }
            
            if (processos[id].created == false) {
                if (pthread_create (&threads[id], NULL, work, (void *) &id)) {
                    printf ("ERRO ao criar a thread!");
                    exit (1);
                }

                processos[id].created = true;
                changed = true;
                running = aux;
            }

            if (changed == true) {
                pthread_mutex_unlock (&sem[id]);
                running = aux;
                changed = false;
            }
            
            usleep (1000000);
            relogio ++;
            processos[id].runCount ++;
            diminuiDt (1);

            if ((int) processos[id].runCount >= processos[id].dt) {
                processos[id].finishedDef = true;
                processos[id].finishedOp = true;

                if (pthread_join (threads[id], NULL)) {
                    printf ("ERRO ao esperar o término da thread %d!\n", id);
                    exit (1);
                }

                pthread_detach (threads[id]);
                procFinalizados ++;
                processos[id].tf = relogio;

                if (processos[id].tf > processos[id].deadline)
                    deadLineNotCump ++;

                if (paramD)
                    fprintf (stderr, "O processo [%s] finalizou: [tf - %d] | [tr - %d]\n", processos[id].nome, processos[id].tf, processos[id].tf - processos[id].start);

                changed = true;
                dequeue ();

                if (!emptyFila ()) {
                    mudaContexto ++;
                    if (paramD)
                        fprintf (stderr, "Nova mudança de contexto. Total de mudanças até agora: %d\n", mudaContexto);
                }
            }
        }

        else {
            usleep (1000000);
            relogio ++;
        }

        newEntry = false;
    }

    freeFila ();
    return NULL;
}

// ESCALONADOR ROUND-ROBIN
void * rr (void * arguments) {
    pthread_t threads[nProc];
    Processo aux;
    int quantum = 200000;
    int procFinalizados = 0;
    int relogio = 0;
    int cont = 0;
    int interval = 1000000 / quantum;
    float incremento = (float) quantum / 1000000;
    int id;

    createFila (nProc);
    while (procFinalizados != nProc) {
        if (cont % interval == 0) {
            while (procAtual < nProc && tempos[relogio] != 0) {
                if (paramD)
                    fprintf (stderr, "O processo [%s] chegou no sistema: [t0 - %d] | [dt - %d] | [deadline - %d]\n", processos[procAtual].nome, processos[procAtual].start, processos[procAtual].dt, processos[procAtual].deadline);
                    
                queue (processos[procAtual]);
                procAtual ++;
                tempos[relogio] --;
            }
        }

        if (!emptyFila ()) {
            aux = getIniFila ();
            id = aux.id;

            if (processos[id].created == false) {
                if (pthread_create (&threads[id], NULL, work, (void *) &id)) {
                    printf ("ERRO ao criar a thread %d!\n", id);
                    exit (1);
                }
                
                processos[id].created = true;
            }

            pthread_mutex_unlock (&sem[id]);
        
            usleep (quantum);
            cont ++;
            if (cont % interval == 0)
                relogio ++;

            processos[id].runCount += incremento;
            processos[id].runCount = round (processos[id].runCount * 10) / 10;

            if ((int) processos[id].runCount >= processos[id].dt) {
                processos[id].finishedDef = true;
                processos[id].finishedOp = true;

                if (pthread_join (threads[id], NULL)) {
                    printf ("ERRO ao esperar o término da thread %d!\n", id);
                    exit (1);
                }

                pthread_detach (threads[id]);
                procFinalizados ++;
                processos[id].tf = relogio;

                if (processos[id].tf > processos[id].deadline)
                    deadLineNotCump ++;

                if (paramD)
                    fprintf (stderr, "O processo [%s] finalizou: [tf - %d] | [tr - %d]\n", processos[id].nome, processos[id].tf, processos[id].tf - processos[id].start);
            }

            processos[id].finishedOp = true;
            if (processos[id].finishedDef == false) {
                queue (dequeue ());
                if (strcmp (aux.nome, getIniFila ().nome) != 0) {
                    mudaContexto ++;
                    if (paramD)
                        fprintf (stderr, "Nova mudança de contexto. Total de mudanças até agora: %d\n", mudaContexto);
                }
            }
            else {
                dequeue ();
                if (!emptyFila ()) {
                    mudaContexto ++;
                    if (paramD)
                        fprintf (stderr, "Nova mudança de contexto. Total de mudanças até agora: %d\n", mudaContexto);
                }
            }
        }

        else {
            usleep (quantum);
            cont ++;
            if (cont % interval == 0)
                relogio ++;
        }
    }

    freeFila ();
    return NULL;
}

// INICIALIZA A THREAD DO ESCALONADOR
void escInit (int type) {
    pthread_t escalonador;

    // incializa os semáforos das threads lockados
    for (int i = 0; i < nProc; i ++) {
        pthread_mutex_init (&sem[i], NULL);
        pthread_mutex_lock (&sem[i]);
    }

    // inicializa a thread do escalonador
    if (type == 1)
        pthread_create (&escalonador, NULL, fcfs, NULL);
    else if (type == 2)
        pthread_create (&escalonador, NULL, srtn, NULL);
    else if (type == 3)
        pthread_create (&escalonador, NULL, rr, NULL);
 
    // espera até o fim da execução da thread do escalonador
    if (pthread_join (escalonador, NULL)) {
        printf ("ERRO ao aguardar o término da thread do escalonador!\n");
        exit (1);
    };

    // destrói os semáforos
    for (int i = 0; i < nProc; i ++) 
        pthread_mutex_destroy (&sem[i]);
}

int main (int argc, char **argv) {
    type = atoi (argv[1]);
    readFile (argv[2]);
    if (argc == 5 && strcmp (argv[4], "d") == 0)
        paramD = true;

    escInit (type);

    writeFile (argv[3]);

    return 0;
}