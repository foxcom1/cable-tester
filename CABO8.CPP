/* by Fabio Oikawa dos Santos - 26/09/2006 */
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

//#define MASK 0x0B

#define DELAY 10

void exibe_binario(unsigned char i)
{
  register unsigned char t;

//  printf("\n");
  for (t=128;t>0;t=t/2)
	if(i&t) printf("1 ");
	else printf("0 ");
// printf("\n");
}

void inicia()
{
  outportb(CONTROL1,0);
  outportb(CONTROL2,0);
  outportb(DATA1,0);
  outportb(DATA2,0);
//  delay(DELAY);
  outportb(CONTROL2,0x20); //habilita o modo EPP para entrada de dados
}

void escreve(int N)
{
  outportb(DATA1,N);
}

void escreve2(int N)//escreve na porta de controle os pinos 1, 14 e 16
{
  register unsigned char aux;
  //escreve (xxxx)(n3 n2 n1 n0) na porta paralela, nao alterando o msnibble
  aux=(inportb(CONTROL2)&0xf0)|(N^0x0b);//os pinos 0,1 e 3 sao invertidos. 0x0b=0000 1011.
  outportb(CONTROL2,aux);
}

unsigned char le(void)
{
  register unsigned char aux;

  aux=inportb(DATA2);

  return aux;
}

unsigned char le2(void)//le da porta de status os pinos 10, 12, 13 e 15
{
  register unsigned char aux;

  aux=inportb(STATUS2)&0x78; //0x78=0111 1000 (os 1s referem-se as posicoes de ack, paper end, slct out e error usadas da porta de status.
  aux>>=3;//ou entao aux<<1

  return aux;
}

int aprova(void)
{
  printf("\n**APROVADO**\n");
  sound(100);
  delay(100);
  nosound();
  return(0); //aprovado
}

int reprova(void)
{
  printf("\n**REPROVADO**\n");
  sound(1000);
  delay(1000);
  nosound();
  return(1); //reprovado
}

void teste2(void) //funcao que testa o cabo com 32 entradas
{
//  unsigned char maior=0x80; //nibble mais significativo
  unsigned char n=0x08; //nibble mais significativo
  unsigned char chip,i,j,aux;
  int log;
  unsigned char a[4][8][8];

//  while(maior>0x08) //quando maior==0x08 o laco acaba
  printf("\nInsira o cabo padrao. (ESC para sair)");
  if(getch()==27) return;
  printf("\nSalvando dados...");

  while(n) //quando n==0 o laco acaba. laco para o cabo padrao
  {
	chip=(~(n<<4))&0xf0; //todas as entradas sao invertidas.
	for(i=8;i<16;i++)//1000 1001.....1110 1111
	{
		escreve(chip|i);//escreve o sinal teste
//		printf("\n");exibe_binario(chip|i); //apenas para mostrar na tela...
//		delay(DELAY);
		for(j=0;j<8;j++)
		{
			escreve2(j);//escreve na porta de controle somente 3 pinos
//			delay(DELAY);
			a[n>4?0:(3-n/2)][i-8][j]=le2();//le 4 bits (8, porem com 4 zeros) da porta status.
//			printf("\na[%u][%u][%u]= ",n>4?0:(3-n/2),i-8,j);
//			exibe_binario(a[n>4?0:(3-n/2)][i-8][j]);
		}
	}
	n>>=1;
  }

  while(1)//laco para o cabo teste
  {
	printf("\nInsira o cabo teste. (ESC para sair)");
	if(getch()==27) return;
	printf("\nTestando...");

	n=0x08;log=0;
	while(n) //quando n==0 o laco acaba. laco para o cabo padrao
	{
		chip=(~(n<<4))&0xf0; //todas as entradas sao invertidas.
		for(i=8;i<16;i++)//1000 1001.....1110 1111
		{
			escreve(chip|i);//escreve o sinal teste
//			printf("\n");exibe_binario(chip|i); //apenas para mostrar na tela...
//			delay(DELAY);
			for(j=0;j<8;j++)
			{
				escreve2(j);//escreve na porta de controle somente 3 pinos
//				delay(DELAY);
				aux=le2();
//				printf("\nesperado: %u recebido: %u",a[n>4?0:(3-n/2)][i-8][j],aux);
				if (a[n>4?0:(3-n/2)][i-8][j]!=aux)
				{
					log=reprova();
					break;
				}
	//			printf("status: %u",a[j]);
			}
			if (log) break;
		}
		if (log) break;
		n>>=1;
	}
	if (!log) aprova();
  }
}


void teste(void)
{
 unsigned char log=0,i,j,n=0x80; //n=(1000 0000)
 unsigned char A[8],aux;  //lista de char sem sinal

 printf("\nInsira o cabo padrÆo.\nPressione qualquer tecla para continuar ou ESC para sair.\n");
 if(getch()==27) return;

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
	if(getch()==27) return;

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
 teste2();
 finaliza();

// getch();
// teste();
}
