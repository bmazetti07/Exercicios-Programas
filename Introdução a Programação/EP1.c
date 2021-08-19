/*MAC0110 - Introdução à Computação
Aluno: Bruno Mazetti Saito
Numero USP: 11221838*/
#include <stdio.h>
int Jogadas (int x);
int ComparaTab (int codigoatual, int codigoSOS);
int main (){
    int opcao;
    printf ("*** Bem-vindo ao Jogo do SOS! ***\n");
    printf ("Digite 1 se voce deseja comecar, ou 2 caso contrario: ");
    scanf ("%d", &opcao);
    if (opcao == 1)
        Jogadas (1);
    else if (opcao == 2)
        Jogadas (2); 
    return(0);
}
int pot (int x, int n){
    int pot = 1;                         //Função para calcular as potências usadas
    while (n > 0){                       //para detereminar as casas do tabuleiro.
        n--;
        pot = pot * x;
    }
    return (pot);
}
int transform (int letra, int linha, int coluna){
    int soma = 0, newvalue = 0;                        //Função que transforma a letra jogada e as
    if (linha == 1 && coluna == 1)                     //casas de escolha do tabuleiro no código
        newvalue = letra*pot (3, 0);                   //utilizado para montar o tabuleiro.
    else if (linha == 1 && coluna == 2)
        newvalue = letra*pot (3, 1);
    else if (linha == 1 && coluna == 3)
        newvalue = letra*pot (3, 2);
    else if (linha == 2 && coluna == 1)
        newvalue = letra*pot (3, 3);
    else if (linha == 2 && coluna == 2)
        newvalue = letra*pot (3, 4);
    else if (linha == 2 && coluna == 3)
        newvalue = letra*pot (3, 5);
    else if (linha == 3 && coluna == 1)
        newvalue = letra*pot (3, 6);
    else if (linha == 3 && coluna == 2)
        newvalue = letra*pot (3, 7);
    else if (linha == 3 && coluna == 3){
        newvalue = letra*pot (3, 8);
    }
    soma = soma + newvalue;
    return (soma);
}
int tab (int codigotab){
    int i, x, code;                                     //Função que imprime o tabuleiro a partir
    code = codigotab;                                   //do codigo fornecido pela função "transform".
    for (i = 0; i < 9; i++){
        x = codigotab % 3; 
    if (x == 0)
        printf ("     ");
    else if (x == 1)
        printf ("  S  ");
    else if (x == 2)
        printf ("  O  ");    
        if (i != 2 && i != 5 && i != 8)
            printf ("|");
        else if (i != 8)
            printf ("\n-----+-----+-----\n");
        else
            printf ("\n\n");
            codigotab = codigotab / 3;
    }
    return  (code);
}
int player (int tabatual){
    int Letra, linha, coluna, Codigo;                                   //Função para realizar a jogada do usuário transformando
    printf ("Digite sua jogada (1 para 'S' e 2 para 'O'): ");           //a letra, a casa e a coluna de escolha, por meio da função
        scanf ("%d", &Letra);                                           //"transform", em um código que é devolvido pela variável "Codigo".
        printf ("Digite a linha: ");                                    
        scanf ("%d", &linha);
        printf ("Digite a coluna: ");
        scanf ("%d", &coluna);
        printf ("Tabuleiro após sua jogada: \n\n");
        if (tabatual == 0)
            return (transform (Letra, linha, coluna));
        else
            Codigo = tabatual + transform (Letra, linha, coluna);
    return (Codigo);
}
int modulo (int z){
    if (z < 0)                                          //Função utilizada pela função "CPU", para garantir que a linha e a coluna geradas
        z = -z;                                         //"aleatóriamente" serão um número positivo.
    return (z);
}
int CPU(int tabatual){
    int row, column, R, C, TabAnt, loop = 0;            //Função que efetua a jogada do computador
    R = modulo((row) % 3);                              //em que apenas na primeira jogada da partida
    C = modulo((column) % 3);                           //iniciada pelo computador é aleatória
    if (R == 0)                                         //e nas demais segue um mesmo padrão.
        R = 3;
    if (C == 0)
        C = 3;
    printf ("Tabuleiro após minha jogada: \n\n");
    TabAnt = tabatual;
        if (tabatual == 0)                 //<--- Jogada para o caso em que o computador inicia a partida
            return (transform (1, R, C));  //gerar o código do tabuleiro a partir de uma linha e coluna aleatória.
        else {                             //<--- Nas demais jogadas
            while ((tabatual % 3) != 0){   //o computador segue um determinado padrão procurando por casas vazias para jogar.
                tabatual /= 3;             //Esta casa vazia é determinada por uma divisão do código por três e pegando seu resto
                loop ++;}                  //que é dada por 3 elevado ao número de vezes em que esse looping ocorreu.
            TabAnt = TabAnt + pot (1*3, loop);
            return (TabAnt);
        }
}
int ComparaTab (int codigoatual, int codigoSOS){
    int digito1, digito2, contadig = 0, k;                  //Função que compara, casa a casa, o codigo do tabuleiro atual
    for (k = 0; k < 9; k++){                                //com todos os códigos em que ocorre SOS
        digito1 = codigoatual % 3;                          //e devolve, pela variável "contadig" quantas casas em comum esses dois
        digito2 = codigoSOS % 3;                            //códigos possuem.
        codigoatual =  codigoatual / 3;
        codigoSOS = codigoSOS / 3;
        if (digito1 != 0 && digito2 != 0){
            if (digito1 == digito2)
                contadig ++;
        }
    }
    return (contadig);
}
int ContSOS (int codigoatual){
    int cont = 0;                                           //Função que recebe de "ComparaTab" a quantidade de casas em comum
    if (ComparaTab (codigoatual, 16) == 3)                  //que os códigos atual e de casos de SOS tem em comum e caso esse resultado
        cont ++;                                            //seja igual a 3, em que essas casas são possuem "S, O e S" respectivamente 
    if (ComparaTab (codigoatual, 432) == 3)                 //devolve, por meio da variável cont, quantos SOS's o tabuleiro atual possui.
        cont ++;
    if (ComparaTab (codigoatual, 11664) == 3)
        cont ++;
    if (ComparaTab (codigoatual, 784) == 3)
        cont ++;
    if (ComparaTab (codigoatual, 2352) == 3)
        cont ++;
    if (ComparaTab (codigoatual, 7056) == 3)
        cont ++;
    if (ComparaTab (codigoatual, 900) == 3)
        cont ++;
    if (ComparaTab (codigoatual, 6724) == 3)
        cont ++;
    return (cont);
}
int Jogadas (int opcao){                                                              //Função em que é determinada a sucessão de jogadas
    int j = 0, tabatual, newSOS = 0, oldSOS = 0, pontos, PtsPC = 0, PtsPlayer = 0;
    if (opcao == 1){
        printf ("Configuracao atual:\n     |     |\n-----+-----+-----\n     |     |\n-----+-----+-----\n     |     |\n");
        tabatual =  player (0);     //Partida iniciada pelo usuário
        tab (tabatual);
        j++;
        printf ("O tabuleiro tem 0 SOS(s)\nPlacar: Usuario 0 x 0 Computador\n\n");
        while (j < 9){
            if (j != 9){
                tabatual = CPU (tabatual);    //Jogada do computador após a(s) jogada(s) do usuário 
                tab (tabatual);
                j++;
                newSOS = ContSOS (tabatual);
                while (newSOS != oldSOS){                           
                    pontos = newSOS - oldSOS;                       //Ao entrar nesse looping, a função verifica se o número de jogadas, dada por "j"
                    printf ("Marquei %d ponto(s).", pontos);        //ultrapassou 9, já que o tabuleiro possui apenas 9 casas
                    PtsPC += (pontos);                              //em que caso isso não ocorra, o respectivo jogador (usuário ou computador)
                    if (j < 9){                                     //possa jogar mais uma vez
                        printf ("Vou jogar novamente.\n");          
                        printf ("O tabuleiro tem %d SOS(s)\nPlacar: Usuario %d x %d Computador\n\n", newSOS, PtsPlayer, PtsPC);
                        tabatual = CPU (tabatual);
                        tab (tabatual);
                        oldSOS = newSOS;
                        newSOS = ContSOS (tabatual);   
                        j++;                 
                }
                    if (j == 9){
                        oldSOS = newSOS;                            //Se o número de jogadas se igualar a 9, a função apenas verficia a possibilidade
                        newSOS = ContSOS (tabatual);                //de o respectivo jogador ter pontuado, mas não jogue novamente
                        if (newSOS != oldSOS){
                            pontos = newSOS - oldSOS;
                            PtsPC += pontos;
                        }
                    printf ("O tabuleiro tem %d SOS(s)\nPlacar: Usuario %d x %d Computador\n\n", newSOS, PtsPlayer, PtsPC);
                    }
                }
            }
            if (j != 9){ 
            tabatual = player (tabatual);     //Jogada do usuário após a(s) jogada(s) do computador
            tab (tabatual);
            j++;
            newSOS = ContSOS (tabatual);
            while (newSOS != oldSOS){
                pontos = newSOS - oldSOS;                          //Ao entrar nesse looping, a função verifica se o número de jogadas, dada por "j"
                printf ("Voce marcou %d ponto(s).", pontos);       //ultrapassou 9, já que o tabuleiro possui apenas 9 casas
                PtsPlayer += pontos;                               //em que caso isso não ocorra, o respectivo jogador (usuário ou computador)
                if (j < 9){                                        //possa jogar mais uma vez
                    printf ("Jogue novamente.\n");
                    printf ("O tabuleiro tem %d SOS(s)\nPlacar: Usuario %d x %d Computador\n\n", newSOS, PtsPlayer, PtsPC);
                    tabatual = player (tabatual);
                    tab (tabatual);
                    oldSOS = newSOS;
                    newSOS = ContSOS (tabatual);
                    j++;
                }
                else if (j == 9){                                  //Se o número de jogadas se igualar a 9, a função apenas verficia a possibilidade
                    oldSOS = newSOS;                               //de o respectivo jogador ter pontuado, mas não jogue novamente
                    newSOS = ContSOS (tabatual);
                    if (newSOS != oldSOS){
                        pontos = newSOS - oldSOS;
                        PtsPlayer += pontos;
                    }
                    printf ("O tabuleiro tem %d SOS(s)\nPlacar: Usuario %d x %d Computador\n\n", newSOS, PtsPlayer, PtsPC);
                }
            }
            }
        }
        if (PtsPC > PtsPlayer)
            printf ("Eu ganhei\nPlacar final: Usuario %d x %d Computador\n\n", PtsPlayer, PtsPC);
        else if (PtsPC == PtsPlayer)
            printf ("Empatamos.\nPlacar final: Usuario %d x %d Computador\n\n", PtsPlayer, PtsPC);
        else
            printf ("Voce ganhou.\nPlacar final: Usuario %d x %d Computador\n\n", PtsPlayer, PtsPC);
    }
    else if (opcao == 2){
        tabatual = CPU(0);      //Partida iniciada pelo computador
        tab (tabatual);
        j++;
        printf ("O tabuleiro tem 0 SOS(s)\nPlacar: Usuario 0 x 0 Computador\n\n");
        while (j < 9){
            if (j != 9){
                tabatual = player (tabatual);
                tab (tabatual);
                j++;
                newSOS = ContSOS (tabatual);
                while (newSOS != oldSOS){                               //Ao entrar nesse looping, a função verifica se o número de jogadas, dada por "j"
                    pontos = newSOS - oldSOS;                           //ultrapassou 9, já que o tabuleiro possui apenas 9 casas
                    printf ("Voce marcou %d ponto(s).", pontos);        //em que caso isso não ocorra, o respectivo jogador (usuário ou computador)
                    PtsPlayer += (pontos);                              //possa jogar mais uma vez
                    if (j < 9){                                         
                        printf ("Jogue novamente.\n");
                        printf ("O tabuleiro tem %d SOS(s)\nPlacar: Usuario %d x %d Computador\n\n", newSOS, PtsPlayer, PtsPC);
                        tabatual = player (tabatual);
                        tab (tabatual);
                        oldSOS = newSOS;
                        newSOS = ContSOS (tabatual);        
                        j++;
                    }
                    else if (j == 9){
                        oldSOS = newSOS;                                //Se o número de jogadas se igualar a 9, a função apenas verficia a possibilidade
                        newSOS = ContSOS (tabatual);                    //de o respectivo jogador ter pontuado, mas não jogue novamente
                        if (newSOS != oldSOS){
                        pontos = newSOS - oldSOS;
                        PtsPlayer += pontos;
                        }
                    printf ("O tabuleiro tem %d SOS(s)\nPlacar: Usuario %d x %d Computador\n\n", newSOS, PtsPlayer, PtsPC);
                    }
                }
            }
            if (j != 9){ 
            tabatual = CPU (tabatual);
            tab (tabatual);
            j++;
            newSOS = ContSOS (tabatual);
            while (newSOS != oldSOS){
                pontos = newSOS - oldSOS;                               //Ao entrar nesse looping, a função verifica se o número de jogadas, dada por "j"
                printf ("Marquei %d ponto(s).", pontos);                //ultrapassou 9, já que o tabuleiro possui apenas 9 casas
                PtsPC += pontos;                                        //em que caso isso não ocorra, o respectivo jogador (usuário ou computador)
                if (j < 9){                                             //possa jogar mais uma vez
                    printf ("Vou jogar novamente.\n");
                    printf ("O tabuleiro tem %d SOS(s)\nPlacar: Usuario %d x %d Computador\n\n", newSOS, PtsPlayer, PtsPC);
                    tabatual = CPU (tabatual);
                    tab (tabatual);
                    oldSOS = newSOS;
                    newSOS = ContSOS (tabatual);
                    j++;
                }
                else if (j == 9){
                    oldSOS = newSOS;                                    //Se o número de jogadas se igualar a 9, a função apenas verficia a possibilidade
                    newSOS = ContSOS (tabatual);                        //de o respectivo jogador ter pontuado, mas não jogue novamente
                    if (newSOS != oldSOS){
                        pontos = newSOS - oldSOS;
                        PtsPC += pontos;
                    }
                    printf ("O tabuleiro tem %d SOS(s)\nPlacar: Usuario %d x %d Computador\n\n", newSOS, PtsPlayer, PtsPC);
                }
            }
            printf ("O tabuleiro tem %d SOS(s)\nPlacar: Usuario %d x %d Computador\n\n", newSOS, PtsPlayer, PtsPC);
            }
        }
        if (PtsPC > PtsPlayer)
            printf ("\nEu ganhei\nPlacar final: Usuario %d x %d Computador\n\n", PtsPlayer, PtsPC);
        else if (PtsPC == PtsPlayer)
            printf ("\nEmpatamos.\nPlacar final: Usuario %d x %d Computador\n\n", PtsPlayer, PtsPC);
        else
            printf ("\nVoce ganhou.\nPlacar final: Usuario %d x %d Computador\n\n", PtsPlayer, PtsPC);
    }
}
