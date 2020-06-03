/* by Fabio Oikawa dos Santos */
#include <stdio.h>
#include <conio.h>
#include <dos.h>

#define LPT1        0x278
#define DATA1       LPT1+0
#define STATUS1     LPT1+1
#define CONTROL1    LPT1+2

#define LPT2        0x378
#define DATA2       LPT2+0
#define STATUS2     LPT2+1
#define CONTROL2    LPT2+2

#define MASK 0x0B

#define DELAY 50

void inicia()
{
  outportb(CONTROL1,0);
  outportb(CONTROL2,0);
  outportb(DATA1,0);
  outportb(DATA2,0);
  delay(DELAY);
  outportb(CONTROL1,0x20); //habilita o modo EPP para entrada de dados
}

void escreve(int N)
{
  outportb(DATA2,N);
}

unsigned char le(void)
{
  unsigned char ByteRecebido;

  ByteRecebido=inportb(DATA1);

  return ByteRecebido;
}

unsigned char teste(int N) //funcao que testa o cabo com a entrada N
{
  unsigned char aux;
  inicia();
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
 unsigned char log=0,i,j=0x80 //i=(1000 0000)
 unsigned char A[]  //lista de char sem sinal

 printf("\n\n\n\n\n\n");
 printf("Insira o cabo padrÆo. Pressione qualquer tecla para continuar...");
 getch();

 i=j;
 while(i>0)
 {
	escreve(i);
	A[i]=le();
	i>>1;
 }


 while(!kbhit())
 {
	printf("Insira o cabo teste. Pressione qualquer tecla para continuar...");

	i=j;
	while(i>0)
	{
		escreve(i);
		if(A[i]!=le())
		{
			printf("\n\n**REPROVADO**\n\n");
			log=1; //reprovado
			break;
		}
		i>>1;

	}
	if(!log) printf("APROVADO");
 }




// if(teste(0x55)) printf("PASSOU");
// else printf("NAO PASSOU");
// inicia();
// escreve(0xff);

 getch();
 getch();
}
