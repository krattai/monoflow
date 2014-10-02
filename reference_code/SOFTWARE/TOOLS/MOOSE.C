/* Simple Demo of Int 33H Mouse Driver

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/
#include<stdio.h>
#include<dos.h>

union REGS regs;

void cls(void);
void goxy(int,int);

void main(int argc, char *argv[])
{
	int x,y,buttons;

	regs.x.ax = 0;
	int86(0x33,&regs,&regs);

	if(regs.x.ax == 0)
	{
		printf("\nMouse not available\n");
		exit(1);
	}

	cls();
	goxy(30,0);
	printf("%3d buttons",regs.x.bx);
	goxy(45,0);
	puts("Press Both Mouse Buttons to Exit");

	regs.x.ax = 1;
	int86(0x33,&regs,&regs);

	do {
		regs.x.ax = 3;
		int86(0x33,&regs,&regs);
		buttons = regs.x.bx & 3;
		x = regs.x.cx;
		y = regs.x.dx;
		goxy(0,0);
		printf("X = %3d  Y = %3d",x,y);
	} while(buttons != 3);

	regs.x.ax = 2;
	int86(0x33,&regs,&regs);

	cls();
	goxy(0,0);
	puts("Have a Mice Day!");
}

void cls(void)
{
	regs.x.ax  = 0x0600;
	regs.h.bh = 7;
	regs.x.cx = 0;
	regs.h.dh = 24;
	regs.h.dl = 79;
	int86(0x10,&regs,&regs);
}

void goxy(int x, int y)
{
	regs.h.dl = x;
	regs.h.dh = y;
	regs.h.bh = 0;
	regs.h.ah = 2;
	int86(0x10,&regs,&regs);
}