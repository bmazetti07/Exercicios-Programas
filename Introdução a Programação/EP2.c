/*MAC0110 - Introdução à Computação
Aluno: Bruno Mazetti Saito
Numero USP: 11221838*/
#include <stdio.h>
#define c1 1837.749265347637
#define c2 834.288324
double frac (double y){
  double fracionaria;
  fracionaria = y - ((int) y);
  return (fracionaria);
}
double modulo (double z){
    if (z < 0)
        z = -z;
    return (z);
}
double seno (double x){
  double res, termo; 
  int den;
  termo = x;
  den = 1;
  res = x;
  while (modulo (termo) > 0.00000001){
    termo = -termo * ((x*x)/((den +1)*(den + 2)));
    res = res + termo;
    den = den + 2;
  }
  return (res);
}
int main (){
  int n, limiar, pontuacaoPlayer = 0, pontuacaoBanca = 0, vitorias = 0, derrotas = 0;
  double x, carta, sen;
  float taxa;
  printf ("Digite a semente (0 < x < 1): ");
  scanf ("%lf", &x);
  printf ("Digite o numero de simulacoes para cada limiar: ");
  scanf ("%d", &n);
  for (limiar = 12; limiar <= 20; limiar ++){
    printf ("%d ( ", limiar);
    for (int i = 0; i < n; i ++){
      while (pontuacaoPlayer < limiar){                     /*O jogador pega cartas enquanto a sua pontuação for menor que o limiar*/
        x = frac (c1 * modulo (seno (x)) + c2);
        carta = (int) (x * 13 + 1);
        if (carta == 11 || carta == 12 || carta == 13)
          carta = 10;
        pontuacaoPlayer += carta;
      }
      if (pontuacaoPlayer < 21){                            /*Condição em que a banca só joga caso o jogador tenha menos que 21 pontos*/
        while (pontuacaoBanca < pontuacaoPlayer){           /*Caso ocorra, a banca joga até que sua pontuação ultrapasse a do jogador*/
          x = frac (c1 * modulo (seno (x)) + c2);
          carta = (int) (x * 13 + 1);
          if (carta == 11 || carta == 12 || carta == 13)
            carta = 10;
          pontuacaoBanca += carta;
        }
      }
      if (pontuacaoPlayer == 21)                   /*Condição para as vitórias em que o jogador ganha caso tenha exatamente 21 pontos*/
        vitorias ++;
      if (pontuacaoBanca > 21)                     /*Ou se a pontuação da banca ultrapassa os 21 pontos*/
        vitorias ++;
    pontuacaoBanca = 0;
    pontuacaoPlayer = 0;
    }
    taxa = 100 * (float) (vitorias/n);             /*A taxa de vitórias é calculada pela quantidade de vitórias sobre o numero de partidas x100*/
      printf ("%.1f%%) : ", 100 * (float)  vitorias / n);
      for (int cont = 0; cont < 100 * (int) vitorias / n; cont ++)
        printf ("*");
      printf ("\n"); 
    vitorias = 0;   
  }
  return (0);
}