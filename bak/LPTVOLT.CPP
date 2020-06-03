//Programa que mede a tensÆo el‚trica entre 0 … 5V atrav‚s da porta EPP
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
#include <UnitPrincipal.h>
#include <Porta.h>
#include <ClassBinario.h>
#include <conio.h>
#include <dos.h>
#define B0 0x01
#define B1 0x02
#define B2 0x04
#define B3 0x08
#define B4 0x10
#define B5 0x20
#define B6 0x40
#define B7 0x80
#define S3 B3
#define S4 B4
#define S5 B5
#define S6 B6
#define S7 B7
#define BASE     0x378
#define DADOS	 BASE + 0
#define STATUS   BASE + 1
#define CONTROLE BASE + 2
#define EPPDATA  BASE + 4
//-----------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TformPrincipal *formPrincipal;
TPorta *Porta;       //Declara objeto para controle da porta paralela.
TBinario *Binario;   //Declara objeto para converter um int numa string bin ria.
unsigned char ByteADC;  //Vari vel para armazenar o byte recebido atrav‚s da porta paralela.
int TimeOut;            //Vari vel para armazenar o tempo de resposta do circuito.
//---------------------------------------------------------------------------
__fastcall TformPrincipal::TformPrincipal(Tcomponent* Owner)
	: Tform(Owner)
{
}
//----------------------------------------------------------------------------
void __fastcall TformPrincipal::TimeAquisicaoTimer(TObject *Sender)
{
  char buf[7];   //Vari vel para armazenar uma string de n£meros
  //IMPORTANTE: Para ativar um pino no ADC envia um 0 e para desativar 1

  //Inicializa a conversÆo do ADC enviando um pulso Baixo (0) no pino 14 C1,
  //momentaneamente.

  Porta->Envia(CONTROLE,34);    //34  0010-0010 aguarda mais ou menos 100 micro segundos
  Porta->Envia(CONTROLE,32);    //32  0010-0000 habilita a conversÆo.
  TimeOut=0;

  do{  //Loop para verificar o Sinal INTR do ADC0804.
     TimeOut++;
  }
  while(((Porta->Recebe(STATUS)&S3)==S3)&&(TimeOut!=256));  //0000-1000
  if(TimeOut==256)
  {
    formPrincipal->caption="Erro!";
  }
  else
  {                             //35  0010-0011
    Porta->Envia(CONTROLE,35);  //Pino 1 C0 Baixo(0), Habilita a sa¡da(leitura) do ADC
    ByteADC=Porta->Recebe(DADOS); //34 0010-0010
    Porta->Envia(CONTROLE,34);  //Pino 1 C0 Alto(1),Desabilita a sa¡da(leitura) do ADC
    sprintf(buf,"%0.2f",ByteADC*0.0196); //Calcula: Byte recebido x 0.0196
    labeVisor->Caption=buf;  //Exibe valor
  }                                       0
}
//----------------------------------------------------------------------------
void __fastcall TformPrincipal::SpeedButtonSairClick(TObject *Sender)
{
   Porta->Envia(CONTROLE,0);  //Desabilita a leitura EPP
   close();
}
//----------------------------------------------------------------------------
void __fastcall TformPrincipal::SpeedButtonMiniClick(TObject *Sender)
{
   WindowState=wsMinimized;
}
]//----------------------------------------------------------------------------