/*MAC0110 - Introdução à Computação
Aluno: Bruno Mazetti Saito
Numero USP: 11221838*/
#include <stdio.h>
#include <math.h>
void ordem (int v[][4], int tam, int x){
    int i, aux[4], j, troca = 1, k;                         //Função que ordena uma matriz
    for (j = 0; j < tam && troca == 1; j++){                //mudando as linhas de lugar a partir 
        troca = 0;                                          //da coluna "x" de escolha   
        for (i = 0; i < tam - 1 - j; i++)                   //utilizando a lógica de BubbleSort
            if (v[i + 1][x] < v[i][x]){
                troca = 1;
                for (k = 0; k < 4; k++){
                    aux[k] = v[i][k];
                    v[i][k] = v[i + 1][k];
                    v[i + 1][k] = aux[k];
                }
            }
    }
}

int conta (int M[][4], int tam, int x){
    int i, j, cont = 0, v[tam];
    for (i = 0; i < tam; i++)
        v[i] = M[i][x];                             //Função utilizada para contagem de quantos 
    for (i = 0; i < tam; i++){                      //termos diferentes há em uma coluna x da matriz
        for (j = 0; j < tam; j++)
            if (i != j)
                if (v[i] == v[j])
                    v[i] = 0;            
        if (v[i] != 0)    
            cont++;    
    }
  return (cont);
}

int opcao (int M[][4], int tam, int opcao){                                 //Função utilizada com base na escolha feita pel usuário
    int i, j, NullLines = 0, ContUso = 0, usuarios, tempo = 0;              //no menu de escolha inicial
        if (opcao == 1){        
            ordem(M, tam, 3);                                               //Opção que imprime a lista de usuarios ordenada pelo NUSP
            i = 0;
            while (M[i][0] == 0){
                NullLines++;
                i++;
            }
            printf ("\n\nLISTA DE USUARIOS DO SALAO DE LEITURA\n\n");
            ordem(M, tam, 0);
            printf ("   NUSP    Hora de entrada   Tempo de permanencia (minutos) \n");
            for (i = NullLines; i < tam; i++)
                printf (" %8d     %02d:%02d            %4d\n", M[i][0], M[i][1], M[i][2], M[i][3]);
        }
        if (opcao == 2){
            printf ("\n\nVISITAS ORDENADAS PELO TEMPO DE PERMANENCIA\n\n");                     //Opção que imprime a lista de usuarios  
            ordem(M, tam, 3);                                                                   //ordenada pelo tempo de permanência no salão
            printf ("   NUSP    Hora de entrada   Tempo de permanencia (minutos) \n");
            i = 0;
            while (M[i][0] == 0){
                NullLines++;
                i++;
            }
            for (i = tam - 1; i > NullLines - 1; i--)
                printf (" %8d     %02d:%02d            %4d\n", M[i][0], M[i][1], M[i][2], M[i][3]);
        }
        if (opcao == 4){
            ordem(M, tam, 0);                                                                   //Opção que mostra a quantidade de usuarios e
            usuarios = conta(M, tam, 0);                                                        //visitas no salão de leitura
            printf ("\n\nNO PERIODO TIVEMOS %d USUARIOS DIFERENTES E", usuarios);
        }
        if (opcao == 6){
            ordem(M, tam, 3);                                                                   //Opção que mostra a quantidade de usuarios por 
            i = 0;                                                                              //determinado tempo de uso
            while (M[i][0] == 0){
                NullLines++;
                i++;
            }
            i = NullLines;
            printf ("\n\nDISTRIBUICAO DE USUARIOS POR TEMPO DE USO\n\n");
            for (j = 100; j <= 1000; j = j + 100){
                ContUso = 0;
                while (M[i][3] < j && i < tam){
                    if (M[i][3] != 0)
                        ContUso++;
                    i++;
                }
            printf ("\n%d PESSOA(S) FICARAM NO SALAO DE %d A %d MINUTOS", ContUso, j - 100, j);
            }
        }
}

void main (){
    int  j, i, visitas = 0, linhas = 0, NullLines = 0, escolha, usuarios, hora;
    int timer[80][4], dados[80][4], histograma[24][2];
    float tempo = 0, DP = 0;
    FILE * arquivo_de_entrada;
    char nome_do_arquivo [80];

    for (i = 0; i < 80; i++)
        for (j = 0; j < 4; j++){
            dados[i][j] = 0;
            timer[i][j] = 0;
        }
    for (i = 0; i < 24; i++)
        for (j = 0; j < 2; j++){
            histograma[i][1] = 0;
            histograma[i][0] = i;
        }

    printf ("Benvindo ao Sistema Estatístico de Uso do Salão de Estudos\n");
    printf ("Biblioteca 'Carlos Benjamim de Lyra' - IME-USP\n\n");
    printf ("Digite o nome do arquivo de dados: ");
    scanf ("%s", nome_do_arquivo);
    arquivo_de_entrada = fopen (nome_do_arquivo, "r");

    while (!feof(arquivo_de_entrada)){
        for (i = 0; i < 80; i++){
            if (fscanf(arquivo_de_entrada, "%d %d %d:%d", &dados[i][0], &dados[i][1], &dados[i][2], &dados[i][3]));
            if (dados[i][1] != 0)                                                           //Os dados lidos no arquivo foram colocados em uma matriz onde:
                linhas ++;                                                                  //A coluna 0 é referente a entrada ou saída do salão;
        }                                                                                   //A coluna 1 é referente ao NUSP do usuario;
    }                                                                                       //A coluna 2 é referente a hora de entrada no salão;
                                                                                            //E a coluna 3 é referente ao minuto de entrada no salão.
    ordem(dados, linhas, 1);
    usuarios = conta(dados, linhas, 1);                                                     //A partir da matriz ordenada pelo NUSP dos usuarios é feita a
    for (i = 0; i < linhas; i++)                                                            //contagem de visitas do salão dada pela quantidade de entrada (entraousai == 0),
        if (dados[i][0] == 0 || (dados[i][0] == 1 && dados[i][1] != dados[i - 1][1]))       //e pelo caso em que o usuario saiu do salão mas não havia uma entrada registrada
            visitas++;                                                                      //no mesmo dia
    j = 0;
    for (i = 0; i < linhas; i++){       //A partir da matriz ordenada pelo NUSP, pode ocorrer os seguintes casos:
        if (dados[j][0] == 0 && dados[i][1] != dados[i + 1][1]){   //Caso que um usuário entra e o proximo NUSP da lista é diferente, ou seja, ele não sai no mesmo dia
            timer[j][0] = dados[i][1];      //Passagem do NUSP para a matriz com o tempo de permanencia
            timer[j][1] = dados[i][2];      //Passagem da hora de entrada
            timer[j][2] = dados[i][3];      //Passagem do minuto de entrada
            timer[j][3] = ((23 *60) + 59) - ((dados[i][2] * 60) +  dados[i][3]);
        }
        else if (dados[i][0] == 0 && dados[i][1] == dados[i + 1][1]){  //Caso que um usuário entra e o proximo NUSP da lista é igual
            timer[j][0] = dados[i][1];     
            timer[j][1] = dados[i][2];      
            timer[j][2] = dados[i][3]; 
            timer[j][3] = (((dados[i + 1][2] * 60) + dados[i + 1][3]) - ((dados[i][2] * 60) + dados[i][3]));
        }
        else if (dados[i][0] == 1 && dados[i - 1][1] != dados[i][1]){ //Caso que só é registrada a saída, ou seja, a entrada não foi no mesmo dia
            timer[j][0] = dados[i][1];     
            timer[j][1] = dados[i][2];      
            timer[j][2] = dados[i][3]; 
            timer[j][3] = ((dados[i][2] * 60) + dados[i][3]);
        }
        tempo += timer[i][3];
        j++;
    }
    //A matriz timer é dada pelo NUSP na coluna 0, pela hora de entrada na coluna 1, pelo minuto de entrada na coluna 2, e pelo tempo de permanencia na coluna 3 

    ordem (timer, linhas, 3);       //A matriz "timer" é ordenada pelo tempo de permanencia
    i = 0;
    while (timer[i][0] == 0){       //Contagem de quantas linhas foram anuladas, ou seja, quantas linhas tinham um NUSP que já havia aparecido
        NullLines++;
        i++;
    }
    printf ("___________________________________________________________________");
    printf ("\nPossiveis acoes: \n");
    printf ("0 - Sair\n");
    printf ("1 - Lista ordenada pelo NUSP\n");
    printf ("2 - Lista ordenada pelo tempo de visita\n");
    printf ("3 - Histograma do uso do salao de leitura\n");
    printf ("4 - Numero total de usuarios diferentes\n");
    printf ("5 - Tempo medio diario do uso do salao e desvio padrao\n");
    printf ("6 - Distribuicao dos usuarios por tempo de uso\n");
    printf ("Escolha uma opcao: ");
    scanf ("%d", &escolha);
    printf ("___________________________________________________________________\n");
    while (escolha != 0){
        if (escolha == 1)
            opcao(timer, linhas, 1);

        else if (escolha == 2)
            opcao(timer, linhas, 2);

        else if (escolha == 3){
            ordem(timer, linhas, 1);
            j = 0;                                                                      //Opção que imprime o histograma de uso do salão
            for (i = NullLines; i < linhas; i++){                                       //onde a ideia usada para desenvolver o histograma foi
                hora = (int) ((timer[i][1] * 60) + timer[i][2] + timer[i][3]) / 60;     //A variavel hora recebe a divisão inteira do momento que o 
                histograma[j][1] += 1;                                                  //usuario entrou no salão (convertido pra minutos) + o tempo de premanencia
                while (hora > j){                                                       //e então desde o momento de entrada até o momento de saída, é somado 1 na coluna 1
                    histograma[hora][1] += 1;                                           //da matriz histograma (que posui 24 linhas uma pra cada hora do dia),
                    hora --;}                                                           //para as respectivas linhas (horas)
                j++;
            }
            printf ("\n\nHISTOGRAMA DE USO DO SALAO\n\n");                              //E então é impresso uma quantidade de * igual ao valor somado em cada linha da coluna 1
            for (i = 0; i < 24; i++){
                printf ("%02d: |", i);
                while (histograma[i][1] > 0){
                    printf ("*");
                    histograma[i][1] --;}
                printf ("\n");
            }
        }

        else if (escolha == 4){
            opcao(timer, linhas, 4);
            printf (" %d VISITAS\n\n", visitas);
        }

        else if (escolha == 5){
            printf ("\n\nA MEDIA DE PERMANENCIA NO SALAO FOI DE %.2f MINUTOS\n", (tempo/visitas));
            for (i = NullLines; i < linhas; i++){                                       
                DP += powf(timer[i][3] - (tempo/visitas), 2);                           //Opção que mostra a média do tempo de permanencia no salão
            }
            DP = sqrtf ((DP)/visitas);                                                  //Juntamente com o desvio padrão do tempo de uso
            printf ("O DESVIO PADRAO FOI DE %.2f\n", DP);
        }

        else if (escolha == 6){
            opcao(timer, linhas, 6);
        }

        else if (escolha == 7){
            printf ("___________________________________________________________________");
            printf ("\nPossiveis acoes: \n");
            printf ("0 - Sair\n");
            printf ("1 - Lista ordenada pelo NUSP\n");
            printf ("2 - Lista ordenada pelo tempo de visita\n");
            printf ("3 - Histograma do uso do salao de leitura\n");
            printf ("4 - Numero total de usuarios diferentes\n");
            printf ("5 - Tempo medio diario do uso do salao e desvio padrao\n");
            printf ("6 - Distribuicao dos usuarios por tempo de uso\n");
            printf ("___________________________________________________________________\n");
        }
    printf ("\n\nEscolha outra opcao (0 - Sair; 7 - Mostrar o menu novamente): ");
    scanf ("%d", &escolha);
    }
}