/* Esse programa obt‚m os endere‡os das portas de comunica‡Æo instaladas */
/* Por F bio Oikawa dos Santos em 04/02/2006 */

#include <stdio.h>
#include <conio.h>
#include <dos.h>

void main(void)
{
 unsigned int far *ptraddr;	/* Ponteiro para localizar os endere‡os das portas */
 unsigned int address;		/* Endere‡o da porta */
 int i;

 ptraddr=(unsigned int far *)0x00000400; /* Endere‡o base de COM1 lido da BIOS Data Area (0000:0400) */

 for(i=0;i<4;i++)
   {
    address=*ptraddr;
    if(address==0) printf("No port found for COM%d \n",i+1);
    else           printf("Address assigned to COM%d is %Xh\n",i+1,address);
    *ptraddr++;
   }
 getch();
}