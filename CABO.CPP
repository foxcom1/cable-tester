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

//#define MASK 0x0B

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

/*
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
*/

void teste(void)
{
 unsigned char log=0,i,j,n=0x80; //n=(1000 0000)
 unsigned char A[8],aux;  //lista de char sem sinal
 char ch;

 printf("\nInsira o cabo padrÆo.\nPressione qualquer tecla para continuar ou ESC para sair.\n");
 if((ch=getch())==27) return;

 for(i=n,j=0;i>0;i=i>>1,j++)
 {
	escreve(i);
	delay(DELAY);
	A[j]=le();
	printf("escrevo: %u\trecebo: %u\n",i,A[j]);
 }


 while(1)
 {
	escreve(0);

	printf("\nInsira o cabo teste.\nPressione qualquer tecla para continuar ou ESC para sair");
	if((ch=getch())==27) return;

	for(log=0,i=n,j=0;i>0;i=i>>1,j++)
	{
		escreve(i);
		delay(DELAY);
		aux=le();
		printf("\nescrevo: %u\tespero: %u\trecebo: %u",i,A[j],aux);
		if(A[j]!=aux)
		{
			printf("\n**REPROVADO**\n");
			sound(1000);
			delay(1000);
			nosound();
			log=1; //reprovado
			break;
		}
	}
	if(!log)
	{
		printf("\n**APROVADO**\n");
		sound(50);
		delay(1000);
		nosound();
	}
 }
}

void finaliza(void)
{
  outportb(CONTROL1,0);
  outportb(CONTROL2,0);
  outportb(DATA1,0);
  outportb(DATA2,0);
}


// if(teste(0x55)) printf("PASSOU");
// else printf("NAO PASSOU");
// inicia();
// escreve(0xff);

void main(void)
{
 clrscr();
 inicia();
 teste();
 finaliza();
// getch();
// teste();
// while((ch=getch())!=27) printf("ch!=27");
}
