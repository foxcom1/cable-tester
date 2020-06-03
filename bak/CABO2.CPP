/* by Fabio Oikawa dos Santos */
#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define LPT1        0x378
#define DATA1       LPT1+0
#define STATUS1     LPT1+1
#define CONTROL1    LPT1+2

#define LPT2        0x278
#define DATA2       LPT2+0
#define STATUS2     LPT2+1
#define CONTROL2    LPT2+2

#define MASK 0x0B

#define DELAY 5

void escreve(int N)
{
  outportb(CONTROL1,0);
  outportb(DATA1,N);
}

unsigned char le(void)
{
  unsigned char ByteRecebido;

  outportb(CONTROL2,0x20);	//habilita o modo EPP para leitura
  ByteRecebido=inportb(DATA2);
  outportb(CONTROL2,0);

  return ByteRecebido;
}

unsigned char teste(int N) //funcao que testa o cabo com a entrada N
{
  unsigned char aux;

  escreve(N);
  delay(DELAY);
  aux=le();
  if(aux==N)
      return 1;		//passou no teste
  else
      return 0;		//n∆o passou no teste
}

void main(void)
{
 int A=0xFF;
 printf("\n\n\n\n\n\n");

/* if(teste(0)) printf("PASSOU");
 else printf("NAO PASSOU");*/
 escreve(A);
 printf("\n%u",A);
 getch();
}
