#ifndef HEADER_H
#define HEADER_H

// REPRESENTAÇÃO DE CADA PROCESSO
typedef struct processo Processo;
struct processo {
    char nome[30];
    int id;                 // Nome do processo
    int start;              // t0
    int dt;                 // Tempo que o processo deve rodar
    int deadline;           // Tempo final suposto do processo
    int tf;                 // Tempo final real do processo
    float runCount;         // Tempo de execução do processo no momento
    bool created;           // Indica se o processo já foi criado
    bool finishedDef;       // Indica se o processo já foi finalizado
    bool finishedOp;        // Indica se as operações do processo devem cessar ou não
};

/* ------------------------------- */

/* Fila usada nos escalonadores Round Robin, SRTN e FCFS*/

Processo * fila;

int iniFila;                // Guarda o índice do começo da fila
int fimFila;                // Guarda o primeiro espaço vazio da fila
int tamFila;                // Guarda o tamanho da fila
int nFila;                  

// Inicia a fila com o tamanho size
void 
createFila (int size);

// Dá free() na fila
void 
freeFila ();

// Aumenta fila caso necessário
void 
resizeFila ();

// Coloca o elemento do tipo "Processo" na fila
void 
queue (Processo proc);

// Retira e devolve o elemento da fila
Processo 
dequeue ();

// Retorna o primeiro elemento da fila
Processo 
getIniFila ();

// Retorna 1 se a fila está vazia, 0 caso contrário
int 
emptyFila ();

// Imprime a fila
void 
printFila ();

// Ordena a fila com base no valor de execução restante dos processos
void 
sortFila ();

// Diminui o valor de dt dos processos presentes na fila
void 
diminuiDt (int val);

#endif