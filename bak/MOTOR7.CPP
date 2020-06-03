#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>

#define DATA 0x378
#define STATUS DATA+1
#define CONTROL DATA+2

#define ARQ "dados.dat"

#define DELAY 1000
#define TEMPO_TOTAL 10000
#define PASSOS TEMPO_TOTAL/DELAY

#define MASK1 0xF0
#define MASK2 0x0B

//FILE *fp;


void binario(unsigned char M) 		//converte um inteiro em binario
{
 int i,k;

 printf("\n");
 for(i=0;i<8;i++)
    {
     k=M/(0x80);			//guarda o quociente do bit A7 -- (A7)xxx xxxx
     printf("%d",k);
     M=M<<1;				//desloca o byte para esquerda
    }
}

void rot_left(int N) //funcao rotaciona esquerda N passos e copia dados para arquivo
{
 FILE *fp;
 unsigned char aux, Motor=1; 	//0000 0001
 int i, Dados;

 if((fp=fopen(ARQ,"w"))==NULL) 	//tenta abrir arquivo
    {
     printf("\nO arquivo %s  nao pode ser aberto\n","ARQ");
     exit(1);
    }
 outportb(CONTROL,inportb(CONTROL)|0x20);//seta bit 5 do CONTROL PORT (xx1x xxxx)
 for(i=0;i<=N;i++)
    {
//  binario(Motor);
     aux=(inportb(CONTROL)&MASK1)|Motor;//variavel auxiliar
     binario(aux);			//imprime o sinal de saida da CONTROL PORT
     outportb(CONTROL,aux^MASK2);	//move o motor

     delay(DELAY);

     Dados=inportb(DATA);		//le dados da DATA PORT
     fprintf(fp,"%d    %d \n",i,Dados); //copia passo e dados para arquivo
     printf("  %2d    %d",i,Dados);
  //  putc(Dados,fp);

     Motor=Motor<<1;	       		//desloca Motor para esquerda
     if (Motor==16) Motor=1; 		//0001 0000 --> 0000 0001 ; 0010 0000 --> 0000 0010 ...
    }
}

int main(void)
{
// FILE *fp;

 printf("\n\n\n\n\n\n");
 rot_left(PASSOS);
// fclose(fp);
 getch();
}



