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
    int tempoPM;                    // Tempo necessário para completar um metro
    int tempoTotal;                 // Tempo total andado em milissegundos (ms) em cada volta
    int tempoFinal;                 // Tempo acumulado de simulação
    int posFaixa;                   // Indica em qual faixa o ciclista está na pista (posição i)
    int posComp;                    // Posição do ciclista na pista (posição j)
    int voltasCompletas;            // Número de voltas completadas até o momento pelo ciclista
} Ciclista;

// VARIÁVEIS GLOBAIS
int d;                              // Comprimento da pista
int n;                              // Número total de ciclistas
bool debug;                         // Opção de debug
int tempo;                          // Intervalo de tempo em que a simulação ocorre
int **pista;                        // Matriz para representação da pista
Ciclista *ciclistas;                // Vetor de ciclistas
bool *competindo;                   // Variável de estado para término ou continuação das threads dos ciclistas
CelLista *ranking;                  // Vetor de lista que indica a posição de cada ciclista naquela volta
int voltaAtual;                     // Indica qual a volta atual
int *colocaoFinal;                  // Vetor que indica a colocação final de cada um dos ciclistas
int **quebrados;                    // Guarda quais e quando os ciclistas quebraram durante a corrida 
_Atomic long int nCiclistas;        // Número total de ciclistas no momento
_Atomic long int CPF[10];           // Indica quantos ciclistas estão presentes em cada faixa (Ciclistas Por Faixa)

// SEMÁFOROS PARA CONTROLE DA SEÇÃO CRÍTICA
pthread_mutex_t **sem;              // Matriz de semáforos para controle da pista
pthread_mutex_t semLista;           // Semáforo para manejar o vetor ranking

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

// IMPRIME OS RANKINGS FINAIS E OS TEMPOS DE SIMULAÇÃO DE CADA UM DOS CICLISTAS
void printFinal () {
    printf ("-------------------- SITUAÇÃO FINAL ------------------------------\n\n");
    printf ("   CICLISTAS   |       COLOCAÇÃO       |   TEMPO DE SIMULAÇÃO (MS)\n");
    printf ("---------------+-----------------------+--------------------------\n");
    for (int i = 0; i < n; i ++) {
        if (quebrados[i][0] == 0)
            printf ("     %4d      |         %3dº          |        %7d\n", i, colocaoFinal[i], ciclistas[i].tempoFinal);
        else
            printf ("     %4d      | QUEBROU NA VOLTA %4d |        %7d\n", i, quebrados[i][1], ciclistas[i].tempoFinal);
        printf ("---------------+-----------------------+--------------------------\n");
    }
}

// IMPRIME A PISTA
void printPista () {
    for (int k = 0; k <= 6 * d; k ++)
        printf ("-");
    printf ("\n");
    for (int i = 0; i < 10; i ++) {
        printf ("|");
        for (int j = 0; j < d; j ++){
            if (pista[i][j] != -1) {
                printf ("\033[0;33m");
                printf ("%5d", pista[i][j]);
                printf ("\033[0m");
                printf ("|");
            }

            else 
                printf ("     |");
        }
        
        printf ("\n");
        for (int k = 0; k <= 6 * d; k ++)
            printf ("-");
        printf ("\n");
    }
}

// GERA UMA PERMUTAÇÃO ALEATÓRIA DE ARRAY PARA INÍCIO DA CORRIDA
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

    if (ciclistas[index].tempoPM == 120) {
        if (x <= 79) {
            ciclistas[index].tempoPM = 60;
        }
    }
    
    else {
        if (x <= 39) {
            ciclistas[index].tempoPM = 120;
        }
    }
}

// CALCULA AS CHANCES DE QUEBRA DO CICLISTA E O RETIRA DA PISTA CASO NECESSÁRIO
void randQuebra (int index) {
    int x;

    x = rand () % 100;

    if (x <= 4 && nCiclistas > 5) {
        int i = ciclistas[index].posFaixa;
        int j = ciclistas[index].posComp;

        competindo[index] = false;
        pista[i][j] = -1;
        CPF[i] --;
        colocaoFinal[index] = nCiclistas;
        nCiclistas --;
        quebrados[index][0] = 1;
        quebrados[index][1] = ciclistas[index].voltasCompletas + 1;

        printf ("O ciclista %d quebrou na volta %d\n", index, ciclistas[index].voltasCompletas + 1);
        arrive[index] = 1;
    }
}

// INICIALIZA A CORRIDA
void initCorrida () {
    // Aloca a matriz pista, 'ranking' e 'quebrados' , o vetor de ciclistas, a matriz de semáforos e os vetores 'competindo', 'continuar', 
    // 'arrive', 'posCiclistas'
    pista = (int **) malloc (10 * sizeof (int *));
    for (int i = 0; i < 10; i ++)
        pista[i] = (int *) malloc (d * sizeof (int));

    quebrados = (int **) malloc (n * sizeof (int *));
    for (int i = 0; i < n; i ++)
        quebrados[i] = (int *) malloc (2 * sizeof (int));

    sem = (pthread_mutex_t **) malloc (10 * sizeof (pthread_mutex_t *));
    for (int i = 0; i < 10; i ++)
        sem[i] = (pthread_mutex_t *) malloc (d * sizeof (pthread_mutex_t));

    ranking = (CelLista *) malloc ((2 * n) * sizeof (CelLista));

    ciclistas = (Ciclista *) malloc (n * sizeof (Ciclista));
    competindo = (bool *) malloc (n * sizeof (bool));
    colocaoFinal = (int *) malloc (n * sizeof (int));
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

    // Inicialização da matriz de quebrados
    // Primeira coluna indica se quebrou ou não e segunda coluna em qual volta ele quebrou
    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < 2; j ++) {
            quebrados[i][j] = 0;
        }
    }

    // Inicialização dos ciclistas e dos vetores 'competindo', 'continuar', 'arrive' e colocaoFinal
    for (int i = 0; i < n; i ++) {
        ciclistas[i].tempoPM = 120;
        ciclistas[i].tempoTotal = 0;
        ciclistas[i].voltasCompletas = -1;
        ciclistas[i].tempoFinal = 0;
        competindo[i] = true;
        colocaoFinal[i] = 0;
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

            k ++;
        }
        if (k % 5 == 0)
            j --;
    }
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

    for (int i = 0; i < n; i ++)
        free (quebrados[i]);

    for (int i = 0; i < 2*n; i ++)
        freeLista (ranking[i].prox);
    free (ranking);

    free (sem);
    free (pista);
    free (quebrados);
    free (ciclistas);
    free (colocaoFinal);
    free (competindo);
    free (continuar);
    free (arrive);
}

// FUNÇÃO DESEMPENHADA POR CADA THREAD DE CICLISTA
void * workCiclista (void * parameters) {
    int id = *( (int *) parameters);

    // A thread continua a rodar enquanto o ciclista está na pista
    while (competindo[id] == true) {
        int i = ciclistas[id].posFaixa;
        int j = ciclistas[id].posComp;
        int firstPos = -1;
        bool andou = true;
        
        if (ciclistas[id].voltasCompletas >= 0)
            ciclistas[id].tempoFinal += tempo;
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
                }
                
                // Não dá para ultrapassar
                else {
                    andou = false;
                    ciclistas[id].tempoTotal -= tempo;
                }
            }
 
            // Atualiza o tempo total andado
            ciclistas[id].tempoTotal = 0;

            // Aleatoriza se quebrará ou não
            if (ciclistas[id].voltasCompletas > 0)
                if (ciclistas[id].posComp == 0 && (ciclistas[id].voltasCompletas + 1) % 6 == 0 && andou == true)
                    randQuebra (id);

            // Libera os semáforos lockados
            pthread_mutex_unlock (&sem[i][j]);
            pthread_mutex_unlock (&sem[i][(j + 1) % d]);
            if (firstPos != -1)
                pthread_mutex_unlock (&sem[firstPos][(j + 1) % d]);

            // Cancela a thread se quebrou o ciclista
            if (competindo[id] == false)
                pthread_cancel (pthread_self());

            // Atualiza a volta do ciclista e muda aleatoriamente sua velocidade se não quebrou
            if (competindo[id] == true && ciclistas[id].posComp == 0 && andou == true) {
                ciclistas[id].voltasCompletas ++;

                if (ciclistas[id].tempoPM != 40)
                    if (ciclistas[id].voltasCompletas > 0)
                        randSpeed (id);

                // Coloca no vetor 'rankings'
                int voltaCompletaAtual = ciclistas[id].voltasCompletas;
                pthread_mutex_lock (&semLista);
                if (voltaCompletaAtual >= 0 && voltaCompletaAtual <= 2 * (n - 1))
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
            usleep (1000);
        continuar[id] = 0;
    }
}

// THREAD COORDENADOR
void * coordenador (void * parameters) {
    int ultimaVolta = 0;        // Guarda qual será a última volta quando tiver certeza
    int ultimosCinco[5];        // Guarda o ID dos ultimos cinco ciclista presentes na pista;
    int andara90 = -2;          // -2 = ninguém andará a 90km/h

    // Inicialização das threads dos ciclistas
    int ids[n];
    bool ehPossivel90 = false;
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
                    usleep (1000);
            arrive[i] = 0;
        }

        // DEBUG
        if (debug) {
            printf ("\033[0;36m");
            printf ("\nITERAÇÃO %d E VOLTA ATUAL %d\n\n", t, voltaAtual);
            printf ("\033[0m");
            printf ("PISTA NESSA ITERAÇÃO\n");
            printPista ();
            printf ("\n");
        }

        // Verifica em quantas voltas anteriores os ciclistas que quebraram estão em relação à voltaAtual
        int qntQuebrados = 0;
        for (int i = 0; i < n; i ++)
            if (quebrados[i][1] > 0 && voltaAtual < quebrados[i][1])
                qntQuebrados ++;

        // Verifica se uma volta acabou
        while (nCiclistas > 1 && (contCiclistas (ranking[voltaAtual].prox) - qntQuebrados) == nCiclistas) {
            
            // Reinicialização da variável 'qntQuebrados'
            qntQuebrados = 0;
            for (int i = 0; i < n; i ++)
                if (quebrados[i][1] > 0 && voltaAtual < quebrados[i][1])
                    qntQuebrados ++;

            // Quando tiver 5 ciclistas, é possível determinar com certeza qual será a última volta
            if (nCiclistas == 5 && (voltaAtual % 2) != 0) {
                ultimaVolta = voltaAtual + 7;

                // Se o primeiro colocado já está correndo as últimas duas voltas, não é possível fazê-lo andar a 90km/h
                int primeiroColocado = firstID (ranking[voltaAtual].prox, quebrados);
                if (ciclistas[primeiroColocado].voltasCompletas >= ultimaVolta - 2 && ciclistas[primeiroColocado].posComp > 0)
                    ehPossivel90 = false;

                else {
                    // Coloca os últimos cinco ciclistas no vetor ultimosCinco
                    CelLista * aux = ranking[voltaAtual].prox;
                    int i = 0;
                    while (aux != NULL) {
                        if (quebrados[aux -> id][0] == 0) {
                            ultimosCinco[i] = aux -> id;
                            i ++;
                        }

                        aux = aux -> prox;
                    }

                    ehPossivel90 = true;
                }
            }

            // Retira o ciclista em último e atualiza o valor de voltaAtual e de nCiclistas
            if (voltaAtual != 0 && (voltaAtual % 2) == 0) {
                int id = lastID (ranking[voltaAtual].prox);

                // Se o último colocado não quebrou posteriormente, tiramos ele da corrida
                if (quebrados[id][0] == 0) {
                    int i = ciclistas[id].posFaixa;
                    int j = ciclistas[id].posComp;

                    // Retira o ciclista que teve sua eliminação atrasada pelo retardatário de outras possíveis listas futuras no 'ranking'
                    int x = voltaAtual, y = ciclistas[id].voltasCompletas;
                    if (x != y) {
                        for (int aux = x + 1; aux <= y && y <= 2 * (n - 1); aux ++)
                            ranking[aux].prox = removeElem (ranking[aux].prox, id);
                    }

                    competindo[id] = false;
                    pista[i][j] = -1;
                    colocaoFinal[id] = nCiclistas;
                    nCiclistas --;
                    CPF[i] --;
                    pthread_cancel (thrCiclistas[id]);
                    pthread_join (thrCiclistas[id], NULL);
                }

                // Se o ciclista quebrou e ainda é o último da voltaAtual
                else {
                    printf ("O ciclista %d quebrou na sua volta %d, mas era para ser eliminado na volta %d.\n", id, quebrados[id][1], voltaAtual);
                }
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
            }
            
            // A cada volta completada, imprime as posições de cada ciclista na voltaAtual
            printf ("\nCOLOCAÇÃO NA VOLTA %d:\n", voltaAtual);
            printLista (ranking[voltaAtual].prox, contCiclistas (ranking[voltaAtual].prox));
            voltaAtual ++;
        }

        // Se é possível que alguém ande a 90km/h, realiza o sorteio para os dois primeiros colocados
        // Nessa condição, o primeiro colocado andará a 90km/h caso o sorteio permita
        if (ehPossivel90 == true) {
            for (int i = 0; i < 5; i ++) {
                int id = ultimosCinco[i];

                if (ciclistas[id].voltasCompletas == ultimaVolta - 2) {
                    int x = rand () % 100;

                    if (x <= 9) {
                        int y = rand () % 100;

                        if (y <= 49) {
                            tempo = 20;
                            ciclistas[id].tempoPM = 40;
                        }

                        else {
                            andara90 = -1;
                            ultimosCinco[i] = -1;
                        }
                    }

                    ehPossivel90 = false;
                    break;
                }
            }
        }

        // Nessa condição, o segundo colocado andará a 90km/h caso o sorteio permita
        if (ehPossivel90 == false && andara90 == -1) {
            for (int i = 0; i < 5; i ++) {
                int id = ultimosCinco[i];

                if (id != -1 && ciclistas[id].voltasCompletas == ultimaVolta - 2) {
                    tempo = 20;
                    ciclistas[id].tempoPM = 40;
                    ehPossivel90 = false;
                    andara90 = -2;

                    break;
                }
            }
        }

        if (nCiclistas > 1) {
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
        }
    }

    // Join naqueles que quebraram para evitar memory_leak
    for (int i = 0; i < n; i ++)
        if (quebrados[i][0] == 1)
            pthread_join (thrCiclistas[i], NULL);

    // Libera a thread do vencedor
    CelLista * aux = ranking[voltaAtual - 1].prox;
    while (aux -> prox != NULL)
        aux = aux -> prox;
    int lastThread = aux -> id;
    printf ("O ciclista %d GANHOU!\n\n", lastThread);
    colocaoFinal[lastThread] = nCiclistas;
    pthread_cancel (thrCiclistas[lastThread]);
    pthread_join (thrCiclistas[lastThread], NULL);
    competindo[lastThread] = false;
}

int main (int argc, char **argv) {
    // Inicialização da corrida e variáveis globais
    char * opcao;
    d = atoi (argv[1]);
    n = atoi (argv[2]);
    tempo = 60;
    nCiclistas = n;
    voltaAtual = 0;
    srand (time (NULL));
    
    printf ("Deseja que o programa use a opção de DEBUG? (S/N) ");
    scanf (" %c", opcao);
    if (strcmp ("S", opcao) == 0)
        debug = true;
    else
        debug = false;

    initCorrida ();

    // Inicialização da thread coordenadora
    pthread_t coord;
    pthread_create (&coord, NULL, coordenador, NULL);

    if (pthread_join (coord, NULL)) {
        printf ("ERRO ao aguardar o término da thread do coordenador!\n");
        exit (1);
    };

    // Imprime colocação final de todos os ciclistas e seus respectivos temposFinais
    printFinal ();

    freeCorrida ();
    return 0;
}