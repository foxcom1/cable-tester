#include <conio.h>
#include <stdio.h>
#include <dos.h>

#define DATA 0x378
#define STATUS DATA+1
#define CONTROL DATA+2

void main(void)
{
  unsigned char RetByte;   //Para armazenar o valor recebido da porta paral.
//  int RetByte;
  clrscr();
  while(!kbhit())          //Faz enquanto nenhuma tecla for pressionada.
  {
     RetByte=inportb(STATUS); //Ler um byte da Porta Paralela (0x379)LPT1.
     RetByte=RetByte>>3;
     gotoxy(1,1);
     printf("%3u (decimal) \n%3x (hexa)",RetByte,RetByte);
  }
//return(0); //comentar esta linha quando conectar na porta paralela.
}

