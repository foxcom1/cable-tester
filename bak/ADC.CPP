#include <stdio.h>
#include <dos.h>
#include <conio.h>

#define data 0x378
#define status 0x379
#define control 0x37a

#define maskcont 0x0B

void write(unsigned short int dados);
void iniciar(void);

void main(void)
{
//char a;
//unsigned short int x;

iniciar();
//for(x=0;x<=15;x++){
clrscr();
//scanf("%x",&x);
write(0xabc);
//}
//getch();
}

void write(unsigned short int dados)
{
//char a;
unsigned short int bh,bl;
//BYTE1/BYTE2    CS    WR1    XFER/WR2, ordem no registro de controle
//1001 1011 1001 ativo o chip e escrevo em latch 1
//1101 desativo o chip
//0001 0011 0001 ativo o chip e escrevo em latch 2
//1101 desativo o chip
//1100 1101 escrevo no registro do chip e o desabilito

bh= (dados&0x0FF0)>>4;
bl= (dados&0x000F)<<4;
printf("%X, %X",bh,bl);
//getche();

outportb(data,bh);

outportb(control,maskcont^0x9);
delay(2);
outportb(control,maskcont^0xB);
delay(2);
outportb(control,maskcont^0x9);

delay(2);
outportb(control,maskcont^0xD);

outportb(data,bl);

outportb(control,maskcont^0x1);
delay(2);
outportb(control,maskcont^0x3);
delay(2);
outportb(control,maskcont^0x1);

delay(2);
outportb(control,maskcont^0xD);
delay(2);
outportb(control,maskcont^0xC);
delay(2);
outportb(control,maskcont^0xD);
}


void iniciar(void)
{
outportb(control,maskcont^0xD);
//outportb(data,0x00);
//outportb(control,maskcont^9);
//outportb(control,maskcont^11);
//outportb(control,maskcont^9);
//outportb(control,makcont^13);
}