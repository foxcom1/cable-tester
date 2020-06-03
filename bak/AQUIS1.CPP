#include <stdio.h>
#include <conio.h>
#include <dos.h>
int main(void)
{
   unsigned char RetByte;
   clrscr();
   while(!kbhit())
   {
      RetByte=inportb(0x379);
      RetByte=RetByte>>3;
       gotoxy(1,1);
       printf("%u ",RetByte);
   }
}