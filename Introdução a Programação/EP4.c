/*MAC0110 - Introdução à Computação
Aluno: Bruno Mazetti Saito
Numero USP: 11221838*/
#include <stdio.h> 
#include <stdlib.h>
int validas = 0, usuario, branco, preto;

void joga (int tabuleiro[8][8], int cor, int l, int c);
void escolhejogada11221838 (int tabuleiro[8][8], int cor, int  *linha, int *coluna);
void ContaPonto (int tabuleiro[8][8]);
void ImprimeTab (int tabuleiro[8][8]);
int podejogar (int tabuleiro[8][8], int cor, int l, int c);
int contavalidas (int tabuleiro[8][8], int cor);

int main (){
    int i, j;
    int tabuleiro[8][8], linha = 0, coluna = 0, cor, cont = 0;
    for (i = 0; i < 8; i++)                                                                       //Arrumação do tabuleiro para a configuração inicial
        for (j = 0; j < 8; j++){
            if ((i == 3 && j == 3) || (i == 4 && j == 4))
                tabuleiro[i][j] = 1;
            else if ((i == 3 && j == 4) || (i == 4 && j == 3))
                tabuleiro[i][j] = -1;
            else
                tabuleiro[i][j] = 0;
            
        }
    printf ("Escolha sua cor [1 (Branco) ou -1(Preto)]: ");
    scanf ("%d", &cor);
    while (cor != 1 && cor != -1){
        printf ("EScolha uma opcao valida. [1 (Branco) ou -1(Preto)]: ");
        scanf ("%d", &cor);
    }
    printf ("\n\nConfiguração inicial: \n\n");
    ImprimeTab (tabuleiro);
    printf ("\n---------------------------------------\n");
    if (cor == -1)
        usuario = 1;
    else
        usuario = 0;
    ContaPonto (tabuleiro);
    while (cont < 60 && branco != 0 && preto != 0){
        if (usuario){
            contavalidas (tabuleiro, cor);
            printf ("Voce tem %d jogadas possiveis\n", validas);
            if (contavalidas (tabuleiro, cor) != 0){
                printf ("Digite sua jogada: ");
                scanf("%d %d", &linha, &coluna);
                if (podejogar (tabuleiro, cor, linha, coluna) == 0){
                    printf ("Sua jogada é inválida, você perdeu \n");
                        if (cor == 1)
                            printf ("As peças pretas ganharam\n");
                        else
                            printf ("As peças brancas ganharam\n");
                    return (0);
                }
                else {
                    joga (tabuleiro, cor, linha, coluna);
                    printf ("\nSua jogada: \n\n");
                    ImprimeTab (tabuleiro);
                    ContaPonto (tabuleiro);
                    printf ("Pontuação:\nBranco %d X %d Preto\n\n", branco, preto);
                    cont ++;
                    if (contavalidas (tabuleiro, -cor) != 0)
                        usuario = 0;
                }
            }
            else {
                printf ("Você não tem jogadas possíveis, perdeu a vez\n");
                usuario = 0;
            }
        }
        else {
            if (contavalidas (tabuleiro, -cor) != 0){
                escolhejogada11221838 (tabuleiro, -cor, &linha, &coluna);
                while (podejogar (tabuleiro, -cor, linha, coluna) != 1)
                    escolhejogada11221838 (tabuleiro, -cor, &linha, &coluna);
                joga (tabuleiro, -cor, linha, coluna);
                printf ("Minha jogada: \n\n");
                ImprimeTab (tabuleiro);
                ContaPonto (tabuleiro);
                printf ("Pontuação:\nBranco %d X %d Preto\n\n", branco, preto);
                cont ++;
                if (contavalidas (tabuleiro, cor) != 0)
                    usuario = 1;
            }
            else {
                printf ("O computador não tem jogadas possíveis, sua vez\n");
                usuario = 1;
            }
        }
    }

    ContaPonto (tabuleiro);
    printf ("Pontuação:\nBranco %d X %d Preto\n", branco, preto);
    if (branco > preto)
        printf ("As peças brancas ganharam\n");
    else if (preto > branco)
        printf ("As peças pretas ganharam\n");
    else
        printf ("Empate");

}


//Função que altera o estado atual do tabuleiro
void joga (int tabuleiro[8][8], int cor, int l, int c){
    int i, j, k, jogou = 0;

    //Alteração na casa de escolha
    tabuleiro[l][c] = cor;


    //Os loopings continuam enquanto não se encontra uma peça da mesma cor
    //da jogada atual, e quando encontrar, as peças intermediarias são
    //revertidas para a cor da jogada

    //Alteração na horizontal direita
    if ((tabuleiro[l][c + 1] != cor) && (tabuleiro[l][c + 1] != 0) && (c + 1 < 8))      
        for (k = c + 2; k < 8 && tabuleiro[l][k] != 0 && jogou == 0; k++)
            if (tabuleiro[l][k] == cor){
                for (k = c + 1; k < 8 && tabuleiro[l][k] != cor; k++)                                                       
                    tabuleiro[l][k] = cor;                                                                                  
                jogou = 1;                                                                                                  
            }
    jogou = 0;

    //Alteração na diagonal superior direita
    if ((tabuleiro[l - 1][c + 1] != cor) && (tabuleiro[l - 1][c + 1] != 0) && (l - 1 > 0) && (c + 1 < 8))
        for (i = l - 2, j = c + 2; i >= 0 && c < 8 && tabuleiro[i][j] != 0 && jogou == 0; i--, j++)
            if (tabuleiro[i][j] == cor){
                for (i = l - 1, j = c + 1; i >= 0 && j < 8 && tabuleiro[i][j] != cor; i--, j++)
                    tabuleiro[i][j] = cor;
                jogou = 1;
            }
    jogou = 0;
    
    //Alteração na vertical para cima
    if ((tabuleiro[l - 1][c] != cor) && (tabuleiro[l - 1][c] != 0) && (l - 1 > 0))
        for (k = l - 2; k >= 0 && tabuleiro[k][c] != 0 && jogou == 0; k--)
            if (tabuleiro[k][c] == cor){
                for (k = l - 1; k >= 0 && tabuleiro[k][c] != cor; k--)
                    tabuleiro[k][c] = cor;
                jogou = 1;
            }
    jogou = 0;
    
    //Alteração na diagonal superior esquerda
    if ((tabuleiro[l - 1][c - 1] != cor) && (tabuleiro[l - 1][c - 1] != 0) && (l - 1 > 0) && (c - 1 > 0))
        for (i = l - 2, j = c - 2; i >= 0 && j >= 0 && tabuleiro[i][j] != 0 && jogou == 0; i--, j--)
            if (tabuleiro[i][j] == cor){
                for (i = l - 1, j = c - 1; i >= 0 && j >= 0 && tabuleiro[i][j] != cor; i--, j--)
                    tabuleiro[i][j] = cor;
                jogou = 1;
            }
    jogou = 0;
    
    //Alteração na horizontal esquerda
    if ((tabuleiro[l][c - 1] != cor) && (tabuleiro[l][c - 1] != 0) && (c - 1 > 0))
        for (k = c - 2; k >= 0 && tabuleiro[l][k] != 0 && jogou == 0; k--)
            if (tabuleiro[l][k] == cor){
                for (k = c - 1; k >= 0 && tabuleiro[l][k] != cor; k--)
                    tabuleiro[l][k] = cor;
                jogou = 1;
            }
    jogou = 0;
    
    //Alteração na diagonal inferior esquerda
    if ((tabuleiro[l + 1][c - 1] != cor) && (tabuleiro[l + 1][c - 1] != 0) && (l + 1 < 8) && (c - 1 > 0))
        for (i = l + 2, j = c - 2; i < 8 && j >= 0 && tabuleiro[i][j] != 0 && jogou == 0; i++, j--)
            if (tabuleiro[i][j] == cor){
                for (i = l + 1, j = c - 1; i < 8 && j >= 0 && tabuleiro[i][j] != cor; i++, j--)
                    tabuleiro[i][j] = cor;
                jogou = 1;
            }
    jogou = 0;

    //Alteração na vertical para baixo
    if ((tabuleiro[l + 1][c] != cor) && (tabuleiro[l + 1][c] != 0) && (l + 1 < 8))
        for (k = l + 2; k < 8 && tabuleiro[k][c] != 0 && jogou == 0; k++)
            if (tabuleiro[k][c] == cor){
                for (k = l + 1; k < 8 && tabuleiro[k][c] != cor; k++)
                    tabuleiro[k][c] = cor;
                jogou = 1;
            }
    jogou = 0;
    
    //Alteração na diagonal inferior direita
    if ((tabuleiro[l + 1][c + 1] != cor) && (tabuleiro[l + 1][c + 1] != 0) && (l + 1 < 8) && (c + 1 < 8))
        for (i = l + 2, j = c + 2; i < 8 && j < 8 && tabuleiro[i][j] != 0 && jogou == 0; i++, j++)
            if (tabuleiro[i][j] == cor){
                for (i = l + 1, j = c + 1; i < 8 && j < 8 && tabuleiro[i][j] != cor; i++, j++)
                    tabuleiro[i][j] = cor;
                jogou = 1;
            }
    jogou = 0;
}

/*     | Função responsável pela escolha da casa da CPU |
       V                                                V                       */
/*************************** COPIE A PARTIR DAQUI **********************************************/
void escolhejogada11221838 (int tabuleiro[8][8], int cor, int  *linha, int *coluna){
    int i, j;
    i = rand () % 8;
    j = rand () % 8;
    if (podejogar(tabuleiro, cor, i, j) == 1){
                *linha = i;
                *coluna = j;
            }
    else {
        while (podejogar (tabuleiro, cor, i, j) != 1){
            i = rand () % 8;
            j = rand () % 8;
        }
        *linha = i;
        *coluna = j;
    }
}
/*************************** COPIE ATÉ AQUI *****************************************************/

int podejogar (int tabuleiro[8][8], int cor, int l, int c){
    int i, j, k;

    //Caso a casa jogada não está já preenchida ou se ela não está fora do tabuleiro
    if (tabuleiro[l][c] != 0 || l < 0 || l > 7 || c < 0 || c > 7)
         return (0);

    //Checagem na horizontal direita
    if ((tabuleiro[l][c + 1] != cor) && (tabuleiro[l][c + 1] != 0) && (c + 1 < 8)){                                      
        for (k = c + 2; k < 8 && tabuleiro[l][k] != 0; k++)
            if (tabuleiro[l][k] == cor)
                return (1);
    }

    //Checagem na diagonal superior direita
    if ((tabuleiro[l - 1][c + 1] != cor) && (tabuleiro[l - 1][c + 1] != 0) && (l - 1 > 0) && (c + 1 < 8)){               
        for (i = l - 2, j = c + 2; i >= 0 && j < 8 && tabuleiro[i][j] != 0; i--, j++)
            if (tabuleiro[i][j] == cor)
                return (1);
    }

    //Checagem na vertical para cima
    if ((tabuleiro[l - 1][c] != cor) && (tabuleiro[l - 1][c] != 0) && (l - 1 > 0)){                                      
        for (k = l - 2; k >= 0 && tabuleiro[k][c] != 0; k--)
            if (tabuleiro[k][c] == cor)
                return (1);
    }

    //Checagem na diagona superior esquerda
    if ((tabuleiro[l - 1][c - 1] != cor) && (tabuleiro[l - 1][c - 1] != 0) && (l - 1 > 0) && (c - 1 > 0)){               
        for (i = l - 2, j = c - 2; i >= 0 && j >= 0 && tabuleiro[i][j] != 0; i--, j--)
            if (tabuleiro[i][j] == cor)
                return (1);
    }

    //Checagem na horizontal esquerda
    if ((tabuleiro[l][c - 1] != cor) && (tabuleiro[l][c - 1] != 0) && (c - 1 > 0)){                                      
        for (k = c - 2; k >= 0 && tabuleiro[l][k] != 0; k--)
            if (tabuleiro[l][k] == cor)
                return (1);
    }

    //Checagem na diagonal inferior esquerda
    if ((tabuleiro[l + 1][c - 1] != cor) && (tabuleiro[l + 1][c - 1] != 0) && (l + 1 < 8) && (c - 1 > 0)){               
        for (i = l + 2, j = c - 2; i < 8 && j >= 0 && tabuleiro[i][j] != 0; i++, j--)
            if (tabuleiro[i][j] == cor)
                return (1);
            
    }

    //Checagem na vertical para baixo
    if ((tabuleiro[l + 1][c] != cor) && (tabuleiro[l + 1][c] != 0) && (l + 1 < 8)){                                      
        for (k = l + 2; k < 8 && tabuleiro[k][c] != 0; k++)
            if (tabuleiro[k][c] == cor)
                return (1);
            
    }

    //Checagem na diagonal inferior direita
    if ((tabuleiro[l + 1][c + 1] != cor) && (tabuleiro[l + 1][c + 1] != 0) && (l + 1 < 8) && (c + 1 < 8)){               
        for (i = l + 2, j = c + 2; i < 8 && j < 8 && tabuleiro[i][j] != 0; i++, j++)
            if (tabuleiro[i][j] == cor)
                return (1);
        }
    return (0);
}

void ContaPonto (int tabuleiro[8][8]){                                                            //Função que conta quantas peças de cada cor há no tabuleiro
    int i, j;
    branco = 0, preto = 0;
    for (i = 0; i < 8; i++)
        for (j = 0; j < 8; j++)
            if (tabuleiro[i][j] == 1)
                branco ++;
            else if (tabuleiro[i][j] == -1)
                preto ++;
}

int contavalidas (int tabuleiro[8][8], int cor){                                                 //Função que conta quantas jogadas validas são possíveis de serem feitas        
    int a, b;                   
    validas = 0;
    for (a = 0; a < 8; a++)
        for (b = 0; b < 8; b++){
            if (podejogar (tabuleiro, cor, a, b) == 1){
                validas ++;
            }
        }
    return (validas);
}

void ImprimeTab (int tabuleiro[8][8]){                                                           //Função para imprimir o tabuleiro
    int i, j;

    printf ("   ");
    for (i = 0; i < 8; i++)
        printf ("%3d ", i);
    printf ("\n");
    for (i = 0; i < 8; i++){
        printf ("   ");
        for (j = 0; j < 8; j++)
            printf ("+---");
        printf ("+\n%d  ", i);
        for (j = 0; j < 8; j++){
            printf ("|");
            if (tabuleiro[i][j] == 0)
                printf ("   ");
            else if (tabuleiro[i][j] == 1)
                printf (" O ");
            else if (tabuleiro[i][j] == -1)
                printf (" X ");
        }
        printf ("|%3d\n", i);
    }
    printf ("   ");
    for (j = 0; j < 8; j++)
        printf ("+---");
    printf ("+\n");

    printf ("   ");
    for (i = 0; i < 8; i++)
        printf ("%3d ", i);
    printf ("\n");

}
