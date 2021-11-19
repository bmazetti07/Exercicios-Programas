#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "header.h"

#define maxProcessos 50 + 7
#define clockTime 1
#define sleep_time 0.1

// VARIÁVEIS GLOBAIS DESTINADOS AO ESCALONADOR FCFS
Processo processos[maxProcessos];                   // Processos lidos do arquivo de trace
bool finished = false;                              // Sinaliza a finalização do processo
bool created = false;                               // Sinaliza o início do processo
//bool finishedLastProc = false;                      // Sinaliza a finalização do último processo
int tempos[maxProcessos];                           // Número de processos existentes em cada t0
int nProc = 0;                                      // Número de processos totais
int procAtual = 0;                                  // Processo atuando no momento

// SEMÁFOROS
pthread_mutex_t sem[maxProcessos];
pthread_mutex_t semEscalonador;

// LÊ O ARQUIVO DE TRACE
void readFile (char * filename) {
    FILE * trace;
    trace = fopen (filename, "r");
               
    while (!feof (trace)) {
        if (fscanf (trace, "%s %d %d %d", processos[nProc].nome, &processos[nProc].start, &processos[nProc].dt, &processos[nProc].deadline) != 4)
            continue;
        printf ("%s %d %d %d\n", processos[nProc].nome, processos[nProc].start, processos[nProc].dt, processos[nProc].deadline);
        processos[nProc].id = nProc;
        processos[nProc].created = false;
        processos[nProc].finishedDef = false;
        processos[nProc].finishedOp = false;
        processos[nProc].runCount = 0.0;
        tempos[processos[nProc].start] ++;
        nProc ++;
    }

    fclose (trace);
}

// FUNÇÃO PARA CADA UM DOS PROCESSOS
void * work (void * parameters) {
    // Arguments parameter = *( (Arguments * ) parameters);
    // int id = parameter.id;
    // int type = parameter.escType;
    int aux = 0;

    created = true;
    while (finished == false) {
        aux ++;
        aux --;
    }

    return NULL;
}

void * work2 (void * parameters) {
    int index = *( (int *) parameters);
    int op = 0;

    while (processos[index].finishedDef == false && !pthread_mutex_lock (&sem[processos[index].id])) {
        printf ("entrei!\n");
        processos[index].finishedOp = false;
        while (processos[index].finishedOp == false) {
            op ++;
            op --;
        }
        printf ("SAI!\n");
    }
    
    printf ("terminei");
    return NULL;
}

// ESCALONADOR FIRST-COME FIRST-SERVED
void * fcfs (void * arguments) {
    pthread_t thread;
    Processo aux;
    int fimTimeProc = 0;
    int relogio = 0;

    createFila ();
    while (procAtual <= nProc) {
        if (processos[nProc - 1].finishedDef)
            break;
    
        while (tempos[relogio]) {
            queue (processos[procAtual]);
            procAtual ++;
            tempos[relogio] --;
        }

        if (emptyFila () == 0) {
            if (created == true && relogio == fimTimeProc) {
                finished = true;
                created = false;
                aux = dequeue ();

                if (strcmp (aux.nome, processos[nProc - 1].nome) == 0)
                    processos[nProc - 1].finishedDef = true;

                if (pthread_join (thread, NULL)) {
                    printf ("ERRO ao esperar o término da thread!");
                    exit (1);
                }

                pthread_detach (thread);
            }

            if (created == false) {
                finished = false;

                if (pthread_create (&thread, NULL, work, NULL)) {
                    printf ("ERRO ao criar a thread!");
                    exit (1);
                }

                aux = getIni ();
                fimTimeProc = aux.dt + relogio;
            }
        }

        usleep (1000000);
        relogio ++;
    }

    freeFila ();
    return NULL;
}

// ESCALONADOR SHORTEST REMAINING TIME NEXT
void * srtn (void * arguments) {
    pthread_mutex_lock (&semEscalonador);

    createLista ();
}

// ESCALONADOR ROUND-ROBIN
void * rr (void * arguments) {
    pthread_t threads[nProc];
    Processo aux;
    int quantum = 50000; // 0.05s
    int procFinalizados = 0;
    int cont = 0;
    double relogio = 0.0;

    createFila ();
    while (procFinalizados != nProc) {
        while (relogio == floor (relogio) && tempos[(int) relogio]) {
            queue (processos[procAtual]);
            procAtual ++;
            tempos[(int) relogio] --;
        }
        printf ("%f %f\n", floor(relogio), relogio);
        if (relogio == (int) relogio)
            printf ("Alalallsladlasdokas\n");
        else 
            printf ("pq????????????????\n");

        if (!emptyFila ()) {
            aux = getIni ();

            if (processos[aux.id].created == false) {
                if (pthread_create (&threads[aux.id], NULL, work2, (void *) &aux.id)) {
                    printf ("ERRO ao criar a thread %d!\n", aux.id);
                    exit (1);
                }
                
                processos[aux.id].created = true;
            }

            pthread_mutex_unlock (&sem[aux.id]);
        
            usleep (quantum);
            relogio += 0.05;
            processos[aux.id].runCount += 0.05;
            printf ("%d %f\n", processos[aux.id].dt, processos[aux.id].runCount);

            if ((int) processos[aux.id].runCount == processos[aux.id].dt) {
                processos[aux.id].finishedDef = true;
                processos[aux.id].finishedOp = true;

                if (pthread_join (threads[processos[aux.id].id], NULL)) {
                    printf ("ERRO ao esperar o término da thread %d\n!", processos[aux.id].id);
                    exit (1);
                }

                pthread_detach (threads[processos[aux.id].id]);
                procFinalizados ++;
            }

            processos[aux.id].finishedOp = true;
            if (processos[aux.id].finishedDef == false)
                queue (dequeue ());
            else
                dequeue ();
        }

        else {
            usleep (quantum);
            relogio += 0.05;
        }
    }
}

// INICIALIZA A THREAD DO ESCALONADOR
void escInit (int type) {
    pthread_t escalonador;

    // incializa os semáforos das threads lockados
    pthread_mutex_init (&semEscalonador, NULL);
    pthread_mutex_lock (&semEscalonador);
    for (int i = 0; i < nProc; i ++) {
        pthread_mutex_init (&sem[i], NULL);
        pthread_mutex_lock (&sem[i]);
    }

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
    pthread_mutex_destroy (&semEscalonador);
    for (int i = 0; i < nProc; i ++) 
        pthread_mutex_destroy (&sem[i]);
}

int main (int argc, char **argv) {
    int type = atoi (argv[1]);
    readFile (argv[2]);

    escInit (type);
  
    return 0;
}