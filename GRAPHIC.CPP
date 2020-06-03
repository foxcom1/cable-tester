#include <graph.h>
#include <stdio.h>
#include <conio.h>

void main(void)
{
_setvideomode(_VRES16COLOR);

_settextcolor(4);
_outtext("Isto est  em vermelho\n");
_getche();

_setvideomode(_DEAFAULTMODE);
}
