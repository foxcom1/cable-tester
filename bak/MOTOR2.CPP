#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#define LPT1 0x378
#define DATA LPT1
#define STATUS LPT1+1
#define CONTROL LPT1+2
#define T 49 		/*tamanho do nome do arquivo de saida*/

int main(int argc , char **argv)
{
int dados;
char nome[T], ch;
int N,t1,t2;
int cont,i;
double x;
FILE *master;

if (argc!=4)
	{
	printf("\nModo correto de uso: %s n delay1 delay2", argv[0]);
	return 0;
	}
N = atoi(argv[1]);
t1 = atoi(argv[2]);
t2 = atoi(argv[3]);

fprintf(stderr, "Informe o nome do arquivo para saida de dados:");
scanf("%s", nome);
if (NULL==(master = fopen(nome, "wb")))
	{
	fprintf(stderr, "Erro na abertura do arquivo %s");
	return 0;
	}
getchar();

/*
 do
 {
 ch = getchar();
 putc(ch,master);
 }while(ch!='0');

printf("\n\ndigite o numero de passos:");
scanf("%d",&N);
printf("\ndigite o intervalo de tempo entre dar o passo e ler:");
scanf("%d",&t1);
printf("\ndigite o intervalo de tempo entre ler e dar o proximo passo:");
scanf("%d",&t2);
*/

for(i=0, cont=0x01;i<N;i++)
	{
	printf("\nEnviando comando para dar o passo %d....",i+1);
	outportb(DATA,cont<<i);     		/* enviando bits pela porta DATA para para mover o motor */
	delay(t1);                            /* intervalo de tempo entre mover o motor e adquirir dados */
	printf("\nLendo a porta....");
	dados=inportb(STATUS)^0x80; 	        /* recuperando dados pela porta STATUS */
	fprintf(stderr,);
	delay(t2);			        /* intervalo de tempo entre adquirir dados e mover o motor */
	}

getch();
return 0;
}