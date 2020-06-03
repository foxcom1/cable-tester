/* by Fabio Oikawa dos Santos */
#include <stdio.h>
#include <iostream.h>
#include <conio.h>
#include <dos.h>
#define BIT0 0x01  // 0000-0001   C0 - Strobe
#define BIT1 0x02  // 0000-0010   C1 - Auto Feed
#define BIT2 0x04  // 0000-0100   C2 - Init
#define BIT3 0x08  // 0000-1000   C3 - Slct In
#define BIT4 0x10  // 0001-0000
#define BIT5 0x20  // 0010-0000  Habilita o modo EPP p/ Leitura/Escrita
#define BIT6 0x40  // 0100-0000
#define BIT7 0x80  // 1000-0000
#define LPT              0x378
#define DATA       LPT+0
#define STATUS     LPT+1
#define CONTROL    LPT+2

void main(void)
{
  int ByteRecebido;
  clrscr();
  outportb(CONTROL,BIT5);
  while(!kbhit())
  {
       ByteRecebido=inportb(DATA);
       gotoxy(10,5);
       printf("Byte recebido: %u  ",ByteRecebido);
//       cout<<"Leitura do registro de Dados: "<<ByteRecebido;
  }
  outportb(CONTROL,0);
//return 0;
}