#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define DATA 0x378
#define STATUS DATA+1
#define CONTROL DATA+2


void main(void)
{
unsign char LED=0x80
while(LED>0)
	{
	outportb(DATA,LED);
	printf("\nPessione uma tecla para acender o proximo LED");
	getch();
	LED=LED>>1;
	}
}


