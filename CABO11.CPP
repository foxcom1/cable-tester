/* by Fabio Oikawa dos Santos - 26/09/2006 */
#include <stdio.h>
#include <stdlib.h>
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

#define TAM_NOME 8
//#define TAM_CODE 256 //8*8*4


typedef struct registro {
   int numero;
   char nome[TAM_NOME];
   unsigned char codigo[4][8][8];
} CABO;


void exibe_binario(unsigned char i)
{
   register unsigned char t;

//  printf("\n");
   for(t = 128; t > 0; t = t/2)
      if (i & t) printf("1 ");
      else printf("0 ");
// printf("\n");
}

long int pot(int b, int e)
{
  register long int aux = 1;

   for(int i = 0; i < e; i++)
      aux *= b;

   return aux;
}

int escreve_arq()
{
   FILE *fp;
   CABO cabo;

   if ((fp = fopen("cabo.dat", "a+b")) == NULL) {
      printf("\nNÆo ‚ poss¡vel abrir arquivo.");
      getch();
      return 1;
   }

   for(int j=1; j<=5; j++){
      cabo.numero=j;
      printf("\nNome:");
      gets(cabo.nome);
      for(int k=0; k<4; k++)
	 for(int l=0; l<8; l++)
	    for(int m=0; m<8; m++)
	       cabo.codigo[k][l][m]=k+l+m;
      fwrite(&cabo, sizeof(CABO), 1, fp);
   }

   fclose(fp);
   return 0;
}

int le_arq()
{
   FILE *fp;
   CABO cabo;

   if ((fp = fopen("cabo.dat", "rb")) == NULL) {
//      printf("\n NÆo ‚ poss¡vel abrir arquivo");
//      getch();
      return 1;
   }

   if (fread(&cabo, sizeof(CABO), 1, fp) != 1)
      printf("\n Erro no arquivo");	//priming read. veja pags. 339 e 340 do Turbo C++ Completo e Total (1991). devido a feof()....

   for(int i = 1; !feof(fp) ; i++) {
      printf("\n%d: %s", cabo.numero, cabo.nome);
      fseek(fp, i*sizeof(CABO), 0);
      fread(&cabo, sizeof(CABO), 1, fp);
   }

   fclose(fp);
   return 0;
}

void inicia()
{
   outportb(CONTROL1, 0);
   outportb(CONTROL2, 0);
   outportb(DATA1, 0);
   outportb(DATA2, 0);
//  delay(DELAY);
   outportb(CONTROL2, 0x20); //habilita o modo EPP para entrada de dados
}

void escreve(int N)
{
   outportb(DATA1,N);
}

void escreve2(int N)//escreve na porta de controle os pinos 1, 14 e 16
{
   register unsigned char aux;
  //escreve (xxxx)(n3 n2 n1 n0) na porta paralela, nao alterando o msnibble
   aux = (inportb(CONTROL2) & 0xf0) | (N^0x0b);//os pinos 0,1 e 3 sao invertidos. 0x0b=0000 1011.
   outportb(CONTROL2,aux);
}

unsigned char le(void)
{
   register unsigned char aux;

   aux = inportb(DATA2);

   return aux;
}

unsigned char le2(void)//le da porta de status os pinos 10, 12, 13 e 15
{
   register unsigned char aux;

   aux = inportb(STATUS2) & 0x78; //0x78=0111 1000 (os 1s referem-se as posicoes de ack, paper end, slct out e error usadas da porta de status.
   aux >>= 3;//ou entao aux<<1

   return aux;
}

void aprova(void)
{
   printf("\n**APROVADO**");
   sound(500);
   delay(500);
   nosound();
//  return(0); //aprovado
}

void reprova(void)
{
   printf("\n**REPROVADO**");
   sound(1000);
   delay(500);
   nosound();
//  delay(500);
//  sound(1000);
//  delay(1000);
//  nosound();
//  delay(500);
//  sound(1000);
//  delay(1000);
//  nosound();

//  return(1); //reprovado
}

void teste2(void) //funcao que testa o cabo com 32 entradas
{
//  unsigned char maior=0x80; //nibble mais significativo
   unsigned char n=0x08, //nibble mais significativo
		 chip,
		 i,
		 j,
		 aux,
		 aux2,
		 t,
		 a[4][8][8];
   int log;

//  while(maior>0x08) //quando maior==0x08 o laco acaba
   printf("\nInsira o cabo padrao. (ESC para sair)");

   if (getch() == 27) return;

   printf("\nSalvando dados...");

   while(n) {//quando n==0 o laco acaba. laco para o cabo padrao
      chip = (~(n<<4)) & 0xf0; //todas as entradas sao invertidas.

      for(i = 8; i < 16; i++) {//1000 1001.....1110 1111
	 escreve(chip|i);//escreve o sinal teste
//		printf("\n");exibe_binario(chip|i); //apenas para mostrar na tela...
	 delay(DELAY);

	 for(j = 0; j < 8; j++) {
	    escreve2(j);//escreve na porta de controle somente 3 pinos
//	    delay(DELAY);
	    a[n>4?0:(3-n/2)][i-8][j] = le2();//le 4 bits (8, porem com 4 zeros) da porta status.
//	    printf("\na[%u][%u][%u]= ",n>4?0:(3-n/2),i-8,j);
//	    exibe_binario(a[n>4?0:(3-n/2)][i-8][j]);
	 }
      }
      n>>=1;
   }

   while(1) {//laco para o cabo teste
      printf("\n\nInsira o cabo teste. (ESC para sair)");

      if (getch() == 27) return;

      printf("\nTestando...");

      n = 0x08; log = 0;

      while(n) { //quando n==0 o laco acaba. laco para o cabo padrao
	 chip = (~(n<<4)) & 0xf0; //todas as entradas sao invertidas.

	 for(i = 8; i < 16; i++) {//1000 1001.....1110 1111
	    escreve(chip|i);//escreve o sinal teste
//	      printf("\n");exibe_binario(chip|i); //apenas para mostrar na tela...
	    delay(DELAY);

	    for(j = 0; j < 8; j++) {
	       escreve2(j);//escreve na porta de controle somente 3 pinos
//		 delay(DELAY);
	       aux = le2();
	       aux2 = a[n>4?0:(3-n/2)][i-8][j]^aux;
//	       printf("\nesperado: %u recebido: %u",a[n>4?0:(3-n/2)][i-8][j],aux);

	       if (aux2) {
		  log = 1;
		  reprova();

		  for(t=0; t<4; t++)
		     if ( aux2 & pot(2, t) )
			printf ("\nErro na via %d", 8*t+j+1);
//					break;
	       }
	//			printf("status: %u",a[j]);
	    }
//			if (log) break;
	 }
//		if (log) break;
      n>>=1;
      }
   if (!log) aprova();
   }
}

void finaliza(void)
{
   outportb(CONTROL1,0);
   outportb(CONTROL2,0);
   outportb(DATA1,0);
   outportb(DATA2,0);
}


void main()
{
clrscr();
// printf("%d",pot(2,10));
// inicia();
// teste2();
// finaliza();
//menu();
// nosound();
//escreve_arq();

le_arq();
getch();
// teste();
}
