#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>

#define DATA 0x378
#define STATUS 0x379
#define CONTROL 0x37a

void main(void)
{
int dados;
int i,N;
int t1,t2;
double x;


printf("\n\ndigite o numero de passos:");
scanf("%d",&N);
printf("\ndigite o intervalo de tempo entre dar o passo e ler:");
scanf("%d",&t1);
printf("\ndigite o intervalo de tempo entre ler e dar o proximo passo:");
scanf("%d",&t2);

for(i=0;i<N;i++)
{
  printf("\nEnviando comando para dar o passo %d....",i+1);
  outportb(DATA,0x01<<i);     		/* enviando bits pela porta DATA para para mover o motor */
  delay(t1);                            /* intervalo de tempo entre mover o motor e adquirir dados */
  printf("\nLendo a porta....");
  dados=inportb(STATUS)^0x80; 	        /* recuperando dados pela porta STATUS */
//  fprintf(stderr)
  delay(t2);			        /* intervalo de tempo entre adquirir dados e mover o motor */
}


getch();
}