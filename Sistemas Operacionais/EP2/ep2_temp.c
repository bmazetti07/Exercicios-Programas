#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include "lista.h"

// REPRESENTAÇÃO DE CADA CICLISTA
typedef struct ciclista {
    int tempoPM;                // Tempo necessário para completar um metro
    int tempoTotal;             // Tempo total andado em milissegundos (ms)
    int posFaixa;               // Indica em qual faixa o ciclista está na pista (posição i)
    int posComp;                // Posição do ciclista na pista (posição j)
    int voltasCompletas;        // Número de voltas completadas até o momento pelo ciclista
} Ciclista;

// VARIÁVEIS GLOBAIS
int d;                          // Comprimento da pista
int n;                          // Número total de ciclistas
int tempo;                      // Intervalo de tempo em que a simulação ocorre
int **pista;                    // Matriz para representação da pista
Ciclista *ciclistas;            // Vetor de ciclistas
bool *competindo;               // Variável de estado para término ou continuação das threads dos ciclistas
CelLista *ranking;              // Vetor de lista que indica a posição de cada ciclista naquela volta
int voltaAtual;                 // Indica qual a volta atual
_Atomic long int nCiclistas;    // Número total de ciclistas no momento
_Atomic long int CPF[10];       // Indica quantos ciclistas estão presentes em cada faixa (Ciclistas Por Faixa)

// SEMÁFOROS PARA CONTROLE DA SEÇÃO CRÍTICA
pthread_mutex_t **sem;          // Matriz de semáforos para controle da pista
pthread_mutex_t semLista;       // Semáforo para manejar o vetor ranking

// BARREIRA DE SINCRONIZAÇÃO
int *continuar;
int *arrive;

// TROCA DOIS VALORES DE POSIÇÃO
void swap (int *a, int *b){
   int t;

   t  = *b;
   *b = *a;
   *a = t;
}

// IMPRIME A PISTA
void printPista () {
    for (int i = 0; i < 10; i ++) {
        for (int j = 0; j < d; j ++){
            if (pista[i][j] != -1) {
                printf ("\033[0;31m");
                printf ("%3d ", pista[i][j]);
                printf ("\033[0m");
            }

            else 
                printf ("%3d ", pista[i][j]);
        }
        
        printf ("\n");
    }
}

// GERAR UMA PERMUTAÇÃO ALEATÓRIA DE ARRAY PARA INÍCIO DA CORRIDA
void shuffle (int array[], int total) {
    for (int i = total - 1; i > 0; i --) {
        int j = rand () % (i + 1);
        swap (&array[i], &array[j]);
    }
}

// ALTERA O VALOR DA VELOCIDADE DOS CICLISTAS
void randSpeed (int index) {
    int x;
    
    x = rand () % 100;
    /* printf ("O x do ciclista %d eh == %d\n", index, x);
    printf ("O ciclista %d está a ", index);
    ciclistas[index].tempoPM == 60 ? printf ("60 km/h\n") : printf ("30 km/h\n");
    printf ("TempoPM do ciclista %d == %d\n", index, ciclistas[index].tempoPM); */

    if (ciclistas[index].tempoPM == 120) {
        if (x <= 79) {
            ciclistas[index].tempoPM = 60;
            printf ("Mudei a velocidade do ciclista %d de 30 para 60\n", index);
        }
    }
    
    else {
        if (x <= 39) {
            ciclistas[index].tempoPM = 120;
            printf ("Mudei a velocidade do ciclista %d de 60 para 30\n", index);
        }
    }
}

// CALCULA AS CHANCES DE QUEBRA DO CICLISTA E O RETIRA DA PISTA CASO NECESSÁRIO
void randQuebra (int index) {
    int x;

    x = rand () % 100;

    if (x <= 4) {
        int i = ciclistas[index].posFaixa;
        int j = ciclistas[index].posComp;

        competindo[index] = false;
        pista[i][j] = -1;
        CPF[i] --;
        nCiclistas --;

        printf ("O ciclista %d quebrou na volta %d\n", index, ciclistas[index].voltasCompletas + 1);
    }
}

// INICIALIZA A CORRIDA
void initCorrida () {
    // Aloca a matriz pista e 'ranking', o vetor de ciclistas, a matriz de semáforos e os vetores 'competindo', 'continuar', 'arrive' e 'posCiclistas'
    pista = (int **) malloc (10 * sizeof (int *));
    for (int i = 0; i < 10; i ++)
        pista[i] = (int *) malloc (d * sizeof (int));

    sem = (pthread_mutex_t **) malloc (10 * sizeof (pthread_mutex_t *));
    for (int i = 0; i < 10; i ++)
        sem[i] = (pthread_mutex_t *) malloc (d * sizeof (pthread_mutex_t));

    ranking = (CelLista *) malloc (2 * n * sizeof (CelLista));

    ciclistas = (Ciclista *) malloc (n * sizeof (Ciclista));
    competindo = (bool *) malloc (n * sizeof (bool));
    continuar = (int *) malloc (n * sizeof (int)); 
    arrive = (int *) malloc (n * sizeof (int));

    // Inicialização da pista, matriz de semáforos e vetor CPF
    for (int i = 0; i < 10; i ++) {
        for (int j = 0; j < d; j ++) {
            pista[i][j] = -1;
            pthread_mutex_init (&sem[i][j], NULL);
        }

        CPF[i] = 0;
    }
    pthread_mutex_init (&semLista, NULL);

    // Inicialização dos ciclistas e dos vetores 'competindo', 'continuar' e 'arrive'
    for (int i = 0; i < n; i ++) {
        ciclistas[i].tempoPM = 120;
        ciclistas[i].tempoTotal = 0;
        ciclistas[i].voltasCompletas = -1;
        competindo[i] = true;
        continuar[i] = 0;
        arrive[i] = 0;
        ranking[i].prox = ranking[i + n].prox = NULL;
    }

    // Coloca os primeiros ciclistas na pista de maneira aleatória
    int auxArray[n];
    for (int i = 0; i < n; i ++)
        auxArray[i] = i;
    shuffle (auxArray, n);

    int k = 0, j = d - 1;
    int aux = 0;
    while (k < n) {
        for (int i = 0; i < 5 && k < n; i ++) {
            if (j % 2 == 0) {
                pthread_mutex_lock (&sem[i][j]);
                pista[i][j] = auxArray[k];
                ciclistas[auxArray[k]].posComp = j;
                ciclistas[auxArray[k]].posFaixa = i;
                CPF[i] ++;
            }

            else {
                pthread_mutex_lock (&sem[i + 5][j]);
                pista[i + 5][j] = auxArray[k];
                ciclistas[auxArray[k]].posComp = j;
                ciclistas[auxArray[k]].posFaixa = i + 5;
                CPF[i + 5] ++;
            }

            if (j == 0)
                ciclistas[auxArray[k]].voltasCompletas --;
            k ++;
        }
        if (k % 5 == 0)
            j --;
    }
    
    printPista ();
}

// DÁ FREE NOS VETORES ALOCADOS
void freeCorrida () {
    for (int i = 0; i < 10; i ++)
        for (int j = 0; j < d; j ++)
            pthread_mutex_destroy (&sem[i][j]);
    pthread_mutex_destroy (&semLista);

    for (int i = 0; i < 10; i ++) {
        free (pista[i]);
        free (sem[i]);
    }

    for (int i = 0; i < 2*n; i ++)
        freeLista (ranking[i].prox);
    free (ranking);

    free (sem);
    free (pista);
    free (ciclistas);
    free (competindo);
    free (continuar);
    free (arrive);
}

// FUNÇÃO DESEMPENHADA POR CADA THREAD DE CICLISTA
void * workCiclista (void * parameters) {
    int id = *( (int *) parameters);

    // A thread continua a rodar enquanto o ciclista está na pista
    while (competindo[id] == true) {
        //printf ("Entrei na thread %d\n", id);
        int i = ciclistas[id].posFaixa;
        int j = ciclistas[id].posComp;
        int firstPos = -1;
        
        ciclistas[id].tempoTotal += tempo;
        
        // Verifica se o ciclista vai andar ou ultrapassar
        if (ciclistas[id].tempoTotal == ciclistas[id].tempoPM) {

            // Espera o ciclista da frente executar
            pthread_mutex_lock (&sem[i][(j + 1) % d]);
            
            // Caso em que a posição seguinte está livre
            if (pista[i][(j + 1) % d] == -1) {
                pista[i][(j + 1) % d] = pista[i][j];
                pista[i][j] = -1;
                ciclistas[id].posComp = (ciclistas[id].posComp + 1) % d;
            }

            else {
                // Verifica se é possível ultrapassar
                for (int x = i; x < 10 && firstPos == -1; x ++) {
                    if (pista[x][j] == -1) {
                        for (int y = i + 1; y < 10 && firstPos == -1; y ++) {
                            pthread_mutex_lock (&sem[y][(j + 1) % d]);
                            if (pista[y][(j + 1) % d] == -1 && CPF[y] < d - 1)
                                firstPos = y;
                            else
                                pthread_mutex_unlock (&sem[y][(j + 1) % d]);
                        }
                    }
                }

                // Dá para ultrapassar
                if (firstPos != -1) {
                    CPF[firstPos] ++;
                    CPF[i] --;
                    pista[firstPos][(j + 1) % d] = pista[i][j];
                    pista[i][j] = -1;
                    ciclistas[id].posFaixa = firstPos;
                    ciclistas[id].posComp = (ciclistas[id].posComp + 1) % d;

                    pthread_mutex_unlock (&sem[firstPos][(j + 1) % d]);
                }
                
                // Não dá para ultrapassar
                else {
                    ciclistas[id].tempoTotal -= tempo;
                }
            }

            // Atualiza o tempo total andado
            ciclistas[id].tempoTotal = 0;

            // Aleatoriza se quebrará ou não
            /* if (ciclistas[id].posComp == 0 && (ciclistas[id].voltasCompletas + 1) % 6 == 0)
                    randQuebra (id); */

            // Libera os semáforos lockados
            pthread_mutex_unlock (&sem[i][j]);
            pthread_mutex_unlock (&sem[i][(j + 1) % d]);

            // Atualiza a volta do ciclista e muda aleatoriamente sua velocidade se não quebrou
            if (competindo[id] == true && ciclistas[id].posComp == 0) {
                //printf ("As voltas completas de %d eh == %d\n", id, ciclistas[id].voltasCompletas);
                ciclistas[id].voltasCompletas ++;

                if (ciclistas[id].voltasCompletas > 0)
                    randSpeed (id);

                // Coloca no vetor 'rankings'
                int voltaCompletaAtual = ciclistas[id].voltasCompletas;
                pthread_mutex_lock (&semLista);
                if (voltaCompletaAtual >= 0)
                    ranking[voltaCompletaAtual].prox = insert (ranking[voltaCompletaAtual].prox, id);
                pthread_mutex_unlock (&semLista);
            }
        }

        // Libera o semáforo de sua posição
        else
            pthread_mutex_unlock (&sem[i][j]);

        // Barreira de Sincronização
        arrive[id] = 1;
        while (continuar[id] == 0)
            usleep (1);
        continuar[id] = 0;
    }
}

// THREAD COORDENADOR
void * coordenador (void * parameters) {
    // Inicialização das threads dos ciclistas
    int ids[n];
    pthread_t thrCiclistas[n];
    for (int i = 0; i < n; i ++) {
        ids[i] = i;
        pthread_create (&thrCiclistas[i], NULL, workCiclista, (void *) &ids[i]);
    }

    // Enquanto a corrida estiver ocorrendo
    for (int t = 0; nCiclistas > 1; t ++) {

        // Barreira de Sincronização
        for (int i = 0; i < n; i ++) {
            if (competindo[i] == true)
                while (arrive[i] == 0)
                    usleep (1);
            arrive[i] = 0;
        }

        printf ("ITERACAO %d e volta atual %d\n\n", t, voltaAtual);
        printPista ();

        // Verifica se uma volta acabou
        if (contCiclistas (ranking[voltaAtual].prox) == nCiclistas) {
            printLista (ranking[voltaAtual].prox, nCiclistas);

            // Retira o ciclista em último e atualiza o valor de voltaAtual e de nCiclistas
            if (voltaAtual != 0 && (voltaAtual % 2) == 0) {
                int id = lastID (ranking[voltaAtual].prox);
                printf ("O último é: %d\n", id);
                int i = ciclistas[id].posFaixa;
                int j = ciclistas[id].posComp;

                competindo[id] = false;
                pista[i][j] = -1;
                nCiclistas --;
                CPF[i] --;
            }

            // Coloca os ciclistas nas faixas mais internas a cada 1 volta completada
            // deixando pelo menos um espaço livre em cada faixa
            if (voltaAtual != 0) {
                for (int i = 1; i < 10; i ++) {
                    for (int j = 0; j < d; j ++) {
                        if (pista[i][j] != -1) {
                            int id = pista[i][j];
                            int x = ciclistas[id].posFaixa;

                            for (int k = 0; k < x; k ++) {
                                if (pista[k][j] == -1 && CPF[k] < d - 1) {
                                    pista[k][j] = pista[i][j];
                                    pista[i][j] = -1;
                                    ciclistas[id].posFaixa = k;
                                    CPF[k] ++;
                                    CPF[i] --;
                                    break;
                                }
                            }
                        }
                    }
                }

                printf ("Mudei a Pista, se liga: \n");
                printPista ();
            }

            voltaAtual ++;
        }

        // Seta os semáforos
        for (int i = 0; i < 10; i ++) {
            for (int j = 0; j < d; j ++) {
                if (pista[i][j] != -1) {
                    pthread_mutex_trylock (&sem[i][j]);
                }
            }
        }

        // Continuar as threads
        for (int i = 0; i < n; i ++)
            continuar[i] = 1;

        printf ("Liberei as threads\n");
    }

    // Libera a thread do vencedor
    int lastThread = ranking[voltaAtual - 1].prox -> prox -> id;
    printf ("A última thread é %d e foi liberada\n", lastThread);
    competindo[lastThread] = false;
}

int main (int argc, char **argv) {
    // Inicialização da corrida e variáveis globais
    d = atoi (argv[1]);
    n = atoi (argv[2]);
    tempo = 60;
    nCiclistas = n;
    voltaAtual = 0;
    srand (1);
    initCorrida ();

    // Inicialização da thread coordenadora
    pthread_t coord;
    pthread_create (&coord, NULL, coordenador, NULL);

    if (pthread_join (coord, NULL)) {
        printf ("ERRO ao aguardar o término da thread do coordenador!\n");
        exit (1);
    };

    freeCorrida ();
    return 0;
}