//este funciona!!
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
  outportb(CONTROL2,0);
  outportb(DATA2,N);
}

unsigned char le(void)
{
  unsigned char ByteRecebido;

  outportb(CONTROL1,0x20);	//habilita o modo EPP para leitura
  ByteRecebido=inportb(DATA1);
  outportb(CONTROL1,0);

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
      return 0;		//nÆo passou no teste
}

void main(void)
{
 printf("\n\n\n\n\n\n");

 if(teste(0)) printf("PASSOU");
 else printf("NAO PASSOU");

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

