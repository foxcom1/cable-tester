#include <dos.h>
#include <conio.h>
#include <stdio.h>

void write(unsigned short int dados);

void main(void)
{
unsigned short int x;

clrscr;

scanf("%x",&x);
write(x);
getche();
}

void write(unsigned short int dados)
{
unsigned short int bh,bl;

bh= (dados&0x0FF0)>>4;
bl= (dados&0x000F)<<4;
printf("%X, %X",bh,bl);
}
