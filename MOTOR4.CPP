/* by Fabio Oikawa dos Santos */
#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define DATA 0x378
#define STATUS DATA+1
#define CONTROL DATA+2

#define DELAY 5
#define TEMPO 2000
#define PASSOS TEMPO/DELAY

#define MASK1 0xF0
#define MASK2 0x0B

void binario(unsigned char M) //converte um inteiro em binario
{
 int i,k;
 printf("\n");
 for(i=0;i<8;i++)
 {
  k=M/(0x80);		//guarda o quociente do bit A7 -- (A7)xxx xxxx
  printf("%d",k);
  M=M<<1;		//desloca o byte para esquerda
 }
}

void rot_left(int N) //funcao rotaciona esquerda N passos
{
 unsigned char aux, Motor=1; //0000 0001
 int i;
 for(i=0;i<=N;i++)
 {
//  binario(Motor);
  aux=(inportb(CONTROL)&MASK1)|Motor;
  binario(aux);
  outportb(CONTROL,aux^MASK2);
  delay(DELAY);
  Motor=Motor<<1;
  if (Motor==16) Motor=1; //0001 0000 --> 0000 0001 ; 0010 0000 --> 0000 0010 ...
 }
}

void main(void)
{
// unsigned char Motor=0x08; //0x08 = 0000 1000
 printf("\n\n\n\n\n\n");
// binario(9);
 rot_left(PASSOS);
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

