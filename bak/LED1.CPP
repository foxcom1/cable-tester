#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define DATA 0x378
#define STATUS DATA+1
#define CONTROL DATA+2


void main(void)
{
unsigned char LED=0x80;
printf("\n\n\n\n\n\n");
while(LED>0)
	{
	outportb(DATA,LED);
	LED=LED>>1;
	printf("\nPessione uma tecla para acender o proximo LED");
	getch();
	}
}


