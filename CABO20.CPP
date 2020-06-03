/* by Fabio Oikawa dos Santos - 28/10/2006 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <ctype.h>

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

#define TAM_NOME 10
//#define MAX_CABO

#define COR_T WHITE //cor do texto do menu
#define COR_F BLACK //cor do fundo do menu
#define COR_T_APR YELLOW + BLINK   //cor do texto da aprovacao
#define COR_F_APR LIGHTBLUE //cor do fundo da aprovacao
#define COR_T_REP WHITE  //cor do tecto da reprovacao
#define COR_F_REP RED //cor do fundo da reprovacao
#define COR_T_TES YELLOW //cor do texto do teste
#define COR_F_TES BLACK //cor do fundo do teste
//#define TAM_CODE 256 //8*8*4

typedef unsigned char matriz[8][8][8];

typedef struct REG_MATR{
   matriz a;
};

typedef struct CABO {
   unsigned char numero;
   char nome[TAM_NOME];
   matriz codigo;
};

void teste(struct REG_MATR); //prototipo
//void teste2(void); //prototipo
int lista(void);
struct REG_MATR padrao();//(matriz *pt)


void exibe_binario(unsigned char i)
{
   register unsigned char t;

   for(t = 128; t > 0; t = t/2)
      if (i & t) printf("1 ");
      else printf("0 ");
}

long int pot(int b, int e)
{
   register long int aux = 1;

   for(int i = 0; i < e; i++)
      aux *= b;

   return aux;
}


void inicia()
{
   outportb(CONTROL1, 0);
   outportb(CONTROL2, 0);
   outportb(DATA1, 0);
   outportb(DATA2, 0);
//  delay(DELAY);
//   outportb(CONTROL2, 0x20); //habilita o modo EPP para entrada de dados
}

void escreveD1(int N)    //escreve na porta de dados da lpt1
{
   outportb(DATA1,N);
}

void escreveD2(int N) //escreve na porta de dados da lpt2
{
   outportb(DATA2,N);
}


void escreveC1(int N)//escreve na porta de controle da lpt1 os pinos 1, 14 e 16
{
   register unsigned char aux;
  //escreve (xxxx)(n3 n2 n1 n0) na porta paralela, nao alterando o msnibble
   aux = (inportb(CONTROL1) & 0xf0) | (N^0x0b);//os pinos 0,1 e 3 sao invertidos. 0x0b=0000 1011.
   outportb(CONTROL1, aux);
}

void escreveC2(int N)//escreve na porta de controle da lpt2 os pinos 1, 14 e 16
{
   register unsigned char aux;
  //escreve (xxxx)(n3 n2 n1 n0) na porta paralela, nao alterando o msnibble
   aux = (inportb(CONTROL2) & 0xf0) | (N^0x0b);//os pinos 0,1 e 3 sao invertidos. 0x0b=0000 1011.
   outportb(CONTROL2, aux);
}

unsigned char leD1(void) //le da porta de dados da lpt1
{
   register unsigned char aux;

   aux = inportb(DATA1);

   return aux;
}

unsigned char leD2(void) //le da porta de dados da lpt1
{
   register unsigned char aux;

   aux = inportb(DATA2);

   return aux;
}

unsigned char leS1(void)//le da porta de status da lpt1 os pinos 10, 12, 13 e 15
{
   register unsigned char aux;

   aux = inportb(STATUS1) & 0x78; //0x78=0111 1000 (os 1s referem-se as posicoes de ack, paper end, slct out e error usadas da porta de status.
   aux >>= 3;//ou entao aux<<1

   return aux;
}


unsigned char leS2(void)//le da porta de status da lpt2 os pinos 10, 12, 13 e 15
{
   register unsigned char aux;

   aux = inportb(STATUS2) & 0x78; //0x78=0111 1000 (os 1s referem-se as posicoes de ack, paper end, slct out e error usadas da porta de status.
   aux >>= 3;//ou entao aux<<1

   return aux;
}

void aprova(void)
{
//   window(10,10,40,30);
   cprintf("\r\n");
   textcolor(COR_T_APR);
   textbackground(COR_F_APR);
   cprintf("**APROVADO**");
   textcolor(COR_T);
   textbackground(COR_F);
//   cprintf("\r\n");
   sound(500);
   delay(500);
   nosound();
//  return(0); //aprovado
}

void reprova(void)
{
   cprintf("\r\n");
   textcolor(COR_T_REP);
   textbackground(COR_F_REP);
   cprintf("**REPROVADO**");
   textcolor(COR_T);
   textbackground(COR_F);
//   cprintf("\r\n");
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


int escreve_cabo(int numero, char *nome, struct REG_MATR codigo)
{
   FILE *fp;
   struct CABO cabo;

   if ((fp = fopen("cabo.dat", "a+b")) == NULL) {
      printf("\nNÆo ‚ poss¡vel abrir arquivo.");
      getch();
      return 1;
   }

   cabo.numero = numero;
   sprintf(cabo.nome, nome);
//   cabo.codigo codigo;
   for(int k = 0; k < 8; k++)
      for(int l = 0; l < 8; l++)
	 for(int m = 0; m < 8; m++)
	     cabo.codigo[k][l][m] = codigo.a[k][l][m];

   fwrite(&cabo, sizeof(struct CABO), 1, fp);

   fclose(fp);
   return 0;
}

struct CABO le_cabo(int num_cabo)
{
   FILE *fp;
   struct CABO cabo;
   int offset;

   if ((fp = fopen("cabo.dat", "rb")) == NULL) {
      printf("\nNÆo ‚ poss¡vel abrir arquivo");
      getch();
      exit(1);//*******************************retornar cabo nulo???!@!!!!
   }

   offset = (num_cabo - 1) * sizeof(struct CABO);
   fseek(fp, offset, 0);
   fread(&cabo, sizeof(struct CABO), 1, fp);

   fclose(fp);

   return cabo;
}

void salva(int num, struct REG_MATR codigo)
{
   char c,
	*ch;

   printf("\nSalvar cabo em arquivo (S ou N)? ");
   while(1) {
      if (tolower(c = getche()) == 's') {
	 printf("\nDigite o nome para o cabo: ");
	 do {
	    gets(ch);
	    if (!strcmp(ch, "")) printf("\nDigite um nome valido: ");
	    else if (strlen(ch) > int(TAM_NOME)) printf("\nDigite um nome com menos de %d caracteres: ", int(TAM_NOME));
	    else escreve_cabo(num, ch, codigo);
	 } while(!strcmp(ch, "") || (strlen(ch) > TAM_NOME));
	 break;
      }//end if
      else if (c == 'n') break;
      else printf("\nS ou N? ");
   }//end while
}

int del_cabo()
{
   FILE *fp,
	*tmp;
   struct CABO cabo;
   char *ch,
	*endptr;
   int  num_cabo,
//	offset,
	log,
	n;
//	j = 1,
//	k = 0;
/*
   if ((fp = fopen("cabo.dat", "a+b")) == NULL) {
      printf("\nNÆo ‚ poss¡vel abrir arquivo.");
      getch();
      return 1;
   }//end if
*/

   while(1) {
      clrscr();
      if (!(n = lista())) {
	 printf("\nNenhum cabo a ser deletado.");
//	 fclose(fp);
	 return 1;
//       teste2();
      }//end if
      else {
	 printf("\nDigite o n£mero do cabo a ser deletado ((q) para sair): ");
	 gets(ch);
	 log = 0;
	 ch = strupr(ch);

	 if (!strcmp(ch, "Q")) {
//	    fclose(fp);
	    return 0;// codigo=padrao();teste(codigo);
	 }//end if
	 else {
	    for(int i = 0; i < strlen(ch); i++)
	       if (!isdigit(ch[i])) {
		  printf("\nEscolha uma op‡Æo v lida.\nPressione qualquer tecla para continuar.");
		  log = 1;
		  getch();
		  break;
//	       return 1;
	       }//end if
	    if (!log) {
	       num_cabo = int(strtol(ch, &endptr, 10));
//	       printf("%d", num_cabo);
	       if ((num_cabo > n) || (num_cabo < 1)) {
		  printf("\nEscolha uma op‡Æo v lida.\nPressione qualquer tecla para continuar.");
		  getch();
//		  break;
	       }//end if
	       else {
		  printf("\nDeletando...");

		  if ((fp = fopen("cabo.dat", "rb")) == NULL) {
		     printf("\nNÆo ‚ poss¡vel abrir arquivo.");
		     getch();
		     return 1;
		  }//end if

		  if ((tmp = fopen("tmp.dat", "wb")) == NULL) {
		     printf("\nNÆo ‚ poss¡vel abrir arquivo.");
		     getch();
		     return 1;
		  }//end if

		  int j = 1, k = 0;
		  do {
		     if (num_cabo != j) {
			fseek(fp, (j - 1) * sizeof(struct CABO), 0);
			fread(&cabo, sizeof(struct CABO), 1, fp);
			cabo.numero = (k + 1);
			fwrite(&cabo, sizeof(struct CABO), 1, tmp);
//			printf("\n%s", cabo[k].nome);
			k++;
		     }//end if
		     j++;
		  } while(j <= n);

		  fclose(fp);
		  fclose(tmp);

		  if (remove("cabo.dat") != 0) printf("\nErro ao deletar o arquivo");
		  if (n == 1) {
		     if (remove("tmp.dat") != 0) printf("\nErro ao deletar o arquivo");
		  }//end if
		  else {
		     if (rename("tmp.dat", "cabo.dat") != 0)
			printf("\nErro ao renomear arquivo");
		  }

/*
		  if (n == 1) {
		     if (remove("cabo.dat") != 0) printf("\nErro ao deletar o arquivo");
		  }//end if
		  else {
		     if ((fp = fopen("cabo.dat", "wb")) == NULL) {
			printf("\nNÆo ‚ poss¡vel abrir arquivo.");
			getch();
			return 1;
		     }//end if

		     for(int i = 0; i < (n - 1); i++)
			fwrite(&cabo[i], sizeof(struct CABO), 1, fp);

		     fclose(fp);
		     fclose(tmp);
		  }//end else
*/
		  return 0;

//		  rewind(fp);
/*
		  fseek(fp, 0, 0);
		  if (num_cabo != 1) {
		     fread(&cabo[0], sizeof(struct CABO), 1, fp);
		  }
		  else {
		  }
		  for(int i = 1; !feof(fp); i++) {

		     fseek(fp, i * sizeof(struct CABO), 0);
		     fread(&cabo[i], sizeof(struct CABO), 1, fp);*/
//		  cabo.numero = num_cabo;

//		  for(i = num_cabo; !feof(fp) ; i++) {
//		     fseek(fp, (i - 1) * sizeof(struct CABO), 0);
//		     fwrite(&cabo, sizeof(struct CABO), 1, fp);

//		     fseek(fp, (i + 1) * sizeof(struct CABO), 0);
//		  }//end for
//		  printf("\nCabo %d deletado. Press ....", num_cabo);
//		  getch();
	       }//end else
	    }//end if
	 }//end else
      }//end else
   }//end while

/*

   cabo.numero = numero;
   sprintf(cabo.nome, nome);
//   cabo.codigo codigo;
   for(int k = 0; k < 4; k++)
      for(int l = 0; l < 8; l++)
	 for(int m = 0; m < 8; m++)
	     cabo.codigo[k][l][m] = codigo[k][l][m];

   fwrite(&cabo, sizeof(CABO), 1, fp);
*/
//   fclose(fp);
//   return 0;

}

int lista() //lista os nomes dos cabos encontrados
{
   FILE *fp;
   struct CABO cabo;

   if ((fp = fopen("cabo.dat", "rb")) == NULL)
//      printf("\n NÆo ‚ poss¡vel abrir arquivo");
//      getch();
      return 0;

   textcolor(COR_T_TES);
   textbackground(COR_F_TES);
   cprintf("\r\nBanco de dados de cabos:");
   textcolor(COR_T);
   textbackground(COR_F);

   if (fread(&cabo, sizeof(struct CABO), 1, fp) != 1)
      printf("\nErro no arquivo");	//priming read. veja pags. 339 e 340 do Turbo C++ Completo e Total (1991). devido a feof()....

   for(int i = 1; !feof(fp) ; i++) {
      printf("\n(%d): Cabo %s", cabo.numero, cabo.nome);
      fseek(fp, i * sizeof(struct CABO), 0);
      fread(&cabo, sizeof(struct CABO), 1, fp);
   }

   fclose(fp);
   return (i - 1);
}

int menu()
{
   char *ch,
	*endptr;
 //	c;
   long num_cabo;
   struct REG_MATR codigo;
   int n,
       log;

   while(1) {
      clrscr();
      if (!(n = lista())) {
	 printf("\nNenhum arquivo de cabos.");
	 printf("\nInsira o cabo padrao. Pressione qualquer tecla para continuar (ESC para sair).");
	 if (getch() == 27) exit(0);
	 else {
//	    printf("\nInsira o cabo padrao e pressione qualquer tecla para continuar (ESC para cancelar)");
//	    if (getch() == 27) ;
	    codigo = padrao();
	    salva(1, codigo);
	    teste(codigo);
	 }//end else
      }//end if
      else {
	 textcolor(COR_T_TES);
	 textbackground(COR_F_TES);
	 cprintf("\r\n\nOpera‡äes:");
	 textcolor(COR_T);
	 textbackground(COR_F);

	 if (n == 1) printf("\n(1): Testar cabo do banco de dados MONTEC.");
	 else printf("\n(1) - (%d): Testar um dos cabos do banco de dados MONTEC.", n);
	 printf("\n(ENTER): Inserir cabo padrÆo novo.");
	 printf("\n(D): Deletar um cabo do banco de dados MONTEC.");
	 printf("\n(S): Sair do programa.");

	 textcolor(COR_T_TES);
	 textbackground(COR_F_TES);
	 cprintf("\r\n\nEscolha uma op‡Æo: ");
	 textcolor(COR_T);
	 textbackground(COR_F);
	 gets(ch);
	 log = 0;
	 ch = strupr(ch);

	 if (!strcmp(ch, "")) {
	    cprintf("Insira o cabo padrao.\r\nPressione qualquer tecla para continuar (ESC para cancelar)");
	    if (getch() == 27) continue;
	    codigo = padrao();
	    salva(n + 1, codigo);
	    teste(codigo);
	 }//end if

	 else if (!strcmp(ch, "D")) del_cabo();
	 else if (!strcmp(ch, "S")) exit(0);
/*	 else if (!strcmp(ch, "")) {
	    printf("\nEscolha uma op‡Æo v lida.\nPress....");
	    getch();
	 }//end else*/
	 else {//verifica se ‚ numero
	    for(int i = 0; i < strlen(ch); i++)
	       if (!isdigit(ch[i])) {
		  printf("\nEscolha uma op‡Æo v lida.");
		  log = 1;
//		  getch();
		  break;
//	          return 1;
	       }
	    if (!log) {
	       num_cabo = int(strtol(ch, &endptr, 10));
	       if (num_cabo > n) {
		  printf("\nEscolha uma op‡Æo de cabo v lida.");
	       }
	       else{
		  cprintf("%d: Cabo %s selecionado", (le_cabo(num_cabo)).numero, (le_cabo(num_cabo)).nome);

		  for(int k = 0; k < 8; k++)
		      for(int l = 0; l < 8; l++)
			 for(int m = 0; m < 8; m++)
			     codigo.a[k][l][m] = (le_cabo(num_cabo)).codigo[k][l][m];

		  teste(codigo);
	       }//end else
	    }//end if
	 }
      }
   cprintf("\r\nPressione qualquer tecla para continuar...");
   getch();
   }
//   return 0;
}


struct REG_MATR padrao()//(matriz *pt)
{
   unsigned char n = 0x08, //nibble menos significativo
		 chip;
   matriz codigo;
   struct REG_MATR A; //registro com matriz
//		 *p[8][8];

//   printf("\nInsira o cabo padrao e pressione qualquer tecla para continuar (ESC para cancelar)");
//   if (getch() == 27) return;

   printf("\nSalvando dados...");

   while(n) {//quando n==0 o laco acaba. laco para o cabo padrao
      chip = (~(n << 4)) & 0xf0; //todas as entradas sao invertidas.

      escreveD2(0x00);//escreve o sinal teste

      for(int i = 8; i < 16; i++) {//1000 1001.....1110 1111
	 escreveD1(chip|i);//escreve o sinal teste
//		printf("\n");exibe_binario(chip|i); //apenas para mostrar na tela...
	 delay(DELAY);

	 for(int j = 0; j < 8; j++) {
	    escreveC1(j);//escreve na porta de controle de lpt1 somente 3 pinos
	    escreveC2(j);//escreve na porta de controle de lpt2 somente 3 pinos
	    codigo[n > 4 ? 0 : (3 - n/2)][i - 8][j] = (0xf0 & (leS2() << 4)) | leS1();//le 8 bits das portas status lpt1 e lpt2.
  //4 chips para escrita (n), 8 pinos num chip para escrita (i), 8 pinos num chip para leitura (j)
	 } //end for
      }//end for
      n >>= 1;
   }

   n = 0x08;

   while(n) {//quando n==0 o laco acaba. laco para o cabo padrao
      chip = (~(n << 4)) & 0xf0; //todas as entradas sao invertidas.

      escreveD1(0x00);//escreve o sinal teste

      for(int i = 8; i < 16; i++) {//1000 1001.....1110 1111
	 escreveD2(chip|i);//escreve o sinal teste
//		printf("\n");exibe_binario(chip|i); //apenas para mostrar na tela...
	 delay(DELAY);

	 for(int j = 0; j < 8; j++) {
	    escreveC1(j);//escreve na porta de controle de lpt1 somente 3 pinos
	    escreveC2(j);//escreve na porta de controle de lpt2 somente 3 pinos
	    codigo[n > 4 ? 4 : (7 - n/2)][i - 8][j] = (0xf0 & (leS2() << 4)) | leS1();//le 8 bits das portas status lpt1 e lpt2.
  //4 chips para escrita (n), 8 pinos num chip para escrita (i), 8 pinos num chip para leitura (j)
  //8 - 0; 4 - 1; 2 - 2; 1 - 3
  //8 - 4; 4 - 5; 2 - 6; 1 - 7
	 } //end for
      }//end for
      n >>= 1;
   }

   for(int k = 0; k < 8; k++)
      for(int l = 0; l < 8; l++)
	 for(int m = 0; m < 8; m++)
	     A.a[k][l][m] = codigo[k][l][m];

//   p = a;
   return A;
}

void teste(struct REG_MATR b)
{
   unsigned char n, //nibble mais significativo
		 chip,
		 aux,
		 aux2;
   char ch;
   int log;

   while(1) {//laco para o cabo teste
//      printf("\n");
      textcolor(COR_T_TES);
      textbackground(COR_F_TES);
      cprintf("\r\n\nInsira o cabo a ser testado.");
      textcolor(COR_T);
      textbackground(COR_F);
      cprintf("\r\nPressione ENTER para continuar. (ESC para sair)");

      while(1) {
	 if ((ch = getch()) == 27) return;
	 else if (ch != 13)
	    cprintf("\r\nPressione ENTER para testar o cabo ou ESC para sair.");
	 else {
	    cprintf("\r\nTestando...");

	    n = 0x08; log = 0;

	    while(n) { //quando n==0 o laco acaba. laco para o cabo padrao
	       chip = (~(n << 4)) & 0xf0; //todas as entradas sao invertidas.

	       escreveD2(0x00);

	       for(int i = 8; i < 16; i++) {//1000 1001.....1110 1111
		  escreveD1(chip | i);//escreve o sinal teste
//	          printf("\n");exibe_binario(chip|i); //apenas para mostrar na tela...
		  delay(DELAY);

		  for(int j = 0; j < 8; j++) {
		     escreveC1(j);//escreve na porta de controle somente 3 pinos
		     escreveC2(j);
//		     delay(DELAY);
		     aux = (0xf0 & (leS2() << 4)) | leS1();
		     aux2 = b.a[n > 4 ? 0 : (3 - n/2)][i - 8][j] ^ aux;

		     if (aux2) {
			log = 1;
			reprova();

//                  printf("\n");
			for(int t = 0; t < 8; t++)
			   if (aux2 & pot(2, t)) {
			      cprintf("\r\n");
			      textcolor(COR_T_REP);
			      textbackground(COR_F_REP);
			      cprintf("Erro na via %d", 8 * t + j + 1);
			      textcolor(COR_T);
			      textbackground(COR_F);
			      cprintf("\r\n");
			      printf("Pressione qualquer tecla para continuar\n");
			      getch();
			   }//end if

//					break;
		     }//end if
	//			printf("status: %u",a[j]);
		  }//end for
//			if (log) break;
	       }//end for
//		if (log) break;
	    n >>= 1;
	    }//end while(n)

	    n = 0x08; //log = 0;

	    while(n) { //quando n==0 o laco acaba. laco para o cabo padrao
	       chip = (~(n << 4)) & 0xf0; //todas as entradas sao invertidas.

	       escreveD1(0x00);

	       for(int i = 8; i < 16; i++) {//1000 1001.....1110 1111
		  escreveD2(chip | i);//escreve o sinal teste
//	          printf("\n");exibe_binario(chip|i); //apenas para mostrar na tela...
		  delay(DELAY);

		  for(int j = 0; j < 8; j++) {
		     escreveC1(j);//escreve na porta de controle somente 3 pinos
		     escreveC2(j);
//		     delay(DELAY);
		     aux = (0xf0 & (leS2() << 4)) | leS1();
		     aux2 = b.a[n > 4 ? 4 : (7 - n/2)][i - 8][j] ^ aux;

		     if (aux2) {
			log = 1;
			reprova();

//                  printf("\n");
			for(int t = 0; t < 8; t++)
			   if (aux2 & pot(2, t)) {
			      cprintf("\r\n");
			      textcolor(COR_T_REP);
			      textbackground(COR_F_REP);
			      cprintf(" Erro na via %d", 8 * t + j + 1);
			      textcolor(COR_T);
			      textbackground(COR_F);
			      cprintf("\r\n");
			      printf("Pressione qualquer tecla para continuar\n");
			      getch();
			   }//end if

//					break;
		     }//end if
	//			printf("status: %u",a[j]);
		  }//end for
//			if (log) break;
	       }//end for
//		if (log) break;
	    n >>= 1;
	    }//end while(n)



	 if (!log) aprova();
	 break;
	 }//end else
      }//end while(1)
   }//end while(1)
}//end teste()


void finaliza(void)
{
   outportb(CONTROL1,0);
   outportb(CONTROL2,0);
   outportb(DATA1,0);
   outportb(DATA2,0);
}


void main()
{
//clrscr();
// printf("%d",pot(2,10));
textcolor(COR_T);
textbackground(COR_F);

inicia();
// teste2();
// finaliza();
//menu();
// nosound();
//escreve_arq(10,"fabio");

//le_arq();
menu();
nosound();

finaliza();
//getch();
// teste();
}
