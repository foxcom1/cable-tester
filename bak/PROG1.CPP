#include <stdio.h>
#include <dos.h>
#include <conio.h>

#define data 0x378
#define status 0x379
#define control 0x37a

void main(void)
{
unsigned int i,j;

i=0;
clrscr;
for(j=0;j<16;j++){
outportb(data,i);
printf("dados enviados a porta 0x%x: 0x%x\n",data,i);
i++;
delay(600);
}
}