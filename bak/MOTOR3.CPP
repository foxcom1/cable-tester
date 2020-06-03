#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define DATA 0x378
#define STATUS DATA+1
#define CONTROL DATA+2

#define DELAY 100
#define MASK 0xF0

void binario(unsigned char M) //converte um inteiro em binario
{
 int i,k;
 printf("\n");
 for(i=0;i<8;i++)
 {
  k=M/(0x80);
  printf("%d",k);
  M=M<<1;
 }
}

void rot_left(int N) //funcao rotaciona esquerda N passos
{
 unsigned char Motor=1; //0000 0001
 int i;
 for(i=0;i<N;i++)
 {
//  binario(Motor);
  binario((inportb(CONTROL)&MASK)|Motor);
  delay(DELAY);
  Motor=(Motor*2)%15; //0001 0000 --> 0000 0001 ; 0010 0000 --> 0000 0010 ...
 }
}



void main(void)
{
// unsigned char Motor=0x08; //0x08 = 0000 1000
 printf("\n\n\n\n\n\n");
// binario(3);
 rot_left(9);
 getch();
}
  /*
while(LED>0)
	{
	outportb(DATA,LED);
	LED=LED>>1;
	printf("\nPessione uma tecla para acender o proximo LED");
	getch();
	}
}
*/

