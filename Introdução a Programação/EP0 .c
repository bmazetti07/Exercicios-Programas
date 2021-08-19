/*Bruno Mazetti Saito 11221838*/

#include <stdio.h>
int main()
{
	int j1, j2, opcao=0;
	while(opcao!=1){
		printf("*** Bem-vindo ao Jokempo ***\n");
		printf("Digite o que o primeiro jogador jogou, (0 - pedra, 1 - tesoura e 2 - papel): ");
		scanf("%d", &j1);
		printf("Digite o que o segundo jogador jogou (0 - pedra, 1 - tesoura e 2 - papel): ");
		scanf("%d", &j2);

		if((j1==0) && (j2==1) ){
			printf("O primeiro jogador ganhou");

		}
		else if((j1==1) && (j2==2) ){
			printf("O primeiro jogador ganhou");

		}
		else if((j1==2) && (j2==0) ){
			printf("O primeiro jogador ganhou");
		}
		else if(j1==j2){
			printf("Empatou!");
		}
		else {
			printf("O segundo jogador ganhou");
		}
		printf(" \nDeseja jogar novamente? (0 - sim, 1 - nao): ");
		scanf("%d", &opcao);

	}
	return(0);

}
