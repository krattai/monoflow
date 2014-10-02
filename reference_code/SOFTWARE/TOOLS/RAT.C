/* RAT.C: Mouse interaction module

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/
#include<stdio.h>
#include<dos.h>
#include"rat.h"

union REGS RATregs;

/* initialize mouse */
void resetrat(RATINFO *ratinfo)
{
	RATregs.x.ax = 4;
	RATregs.x.cx = 640/2;
	RATregs.x.dx = 480/2;
	int86(0x33,&RATregs,&RATregs);
	ratinfo->horiz = RATregs.x.cx;
	ratinfo->verti = RATregs.x.dx;
}

void initrat(RATINFO *ratinfo)
{
	RATregs.x.ax = 0;
	int86(0x33,&RATregs,&RATregs);		/* turn on mouse */

	if(RATregs.x.ax == 0)
	{
		closegraph();
		printf("\nMouse not available\n");
		exit(1);
	}

	RATregs.x.ax = 4;
	RATregs.x.cx = 640/2;
	RATregs.x.dx = 480/2;
	int86(0x33,&RATregs,&RATregs);

	RATregs.x.ax = 3;
	int86(0x33,&RATregs,&RATregs);
	ratinfo->horiz = RATregs.x.cx;
	ratinfo->verti = RATregs.x.dx;
}

/* display mouse */
void raton(void)
{
	RATregs.x.ax = 1;
	int86(0x33,&RATregs,&RATregs);
}

/* hide mouse */
void ratoff(void)
{
	RATregs.x.ax = 2;
	int86(0x33,&RATregs,&RATregs);
}

/* get input from mouse */
int getratinput(RATINFO *ratinfo)
{
	int bx;

	RATregs.x.ax = 3;
	int86(0x33,&RATregs,&RATregs);
/*	if(RATregs.x.bx == 0)
	{
		ratinfo->buttons = LBUTTON;
		ratinfo->horiz = RATregs.x.cx;
		ratinfo->verti = RATregs.x.dx;
		return(HELD);	*/							/* keys on */
/*	} */
	if(RATregs.x.bx == 2)
	{
		ratinfo->buttons = RATregs.x.bx & 7;
/*		ratinfo->buttons = RBUTTON; */
		ratinfo->horiz = RATregs.x.cx;
		ratinfo->verti = RATregs.x.dx;
		return(HELD);								/* keys on */
	}
/*	if(RATregs.x.bx == 2)
	{
		ratinfo->buttons = CBUTTON;
		ratinfo->horiz = RATregs.x.cx;
		ratinfo->verti = RATregs.x.dx;
		return(HELD);	*/							/* keys on */
/*	} */

	/* check press */
	RATregs.x.ax = 5;
	RATregs.x.bx = 0;
	int86(0x33,&RATregs,&RATregs);
	if(RATregs.x.bx)
	{
		ratinfo->buttons = LBUTTON;
		ratinfo->horiz = RATregs.x.cx;
		ratinfo->verti = RATregs.x.dx;
		return(PRESS);
	}

	RATregs.x.ax = 5;
	RATregs.x.bx = 1;
	int86(0x33,&RATregs,&RATregs);
	if(RATregs.x.bx)
	{
		ratinfo->buttons = RBUTTON;
		ratinfo->horiz = RATregs.x.cx;
		ratinfo->verti = RATregs.x.dx;
		return(PRESS);
	}

	RATregs.x.ax = 5;
	RATregs.x.bx = 2;
	int86(0x33,&RATregs,&RATregs);
	if(RATregs.x.bx)
	{
		ratinfo->buttons = CBUTTON;
		ratinfo->horiz = RATregs.x.cx;
		ratinfo->verti = RATregs.x.dx;
		return(PRESS);
	}

	/* check release */
	RATregs.x.ax = 6;
	RATregs.x.bx = 0;
	int86(0x33,&RATregs,&RATregs);
	ratinfo->buttons = RATregs.x.ax & 7;
	if(RATregs.x.bx)
	{
		ratinfo->horiz = RATregs.x.cx;
		ratinfo->verti = RATregs.x.dx;
		return(RELEASE);
	}

	RATregs.x.ax = 6;
	RATregs.x.bx = 1;
	int86(0x33,&RATregs,&RATregs);
	ratinfo->buttons = RATregs.x.ax & 7;
	if(RATregs.x.bx)
	{
		ratinfo->horiz = RATregs.x.cx;
		ratinfo->verti = RATregs.x.dx;
		return(RELEASE);
	}

	RATregs.x.ax = 6;
	RATregs.x.bx = 2;
	int86(0x33,&RATregs,&RATregs);
	ratinfo->buttons = RATregs.x.ax & 7;
	if(RATregs.x.bx)
	{
		ratinfo->horiz = RATregs.x.cx;
		ratinfo->verti = RATregs.x.dx;
		return(RELEASE);
	}

	/* cursor movement only */
	RATregs.x.ax = 3;
	int86(0x33,&RATregs,&RATregs);
	ratinfo->horiz = RATregs.x.cx;
	ratinfo->verti = RATregs.x.dx;
	return(MOVED);
}

/* clear display, text only */
void cls(void)
{
	RATregs.x.ax  = 0x0600;
	RATregs.h.bh = 7;
	RATregs.x.cx = 0;
	RATregs.h.dh = 24;
	RATregs.h.dl = 79;
	int86(0x10,&RATregs,&RATregs);
}

/* go to x,y position, text only */
void goxy(int x, int y)
{
	RATregs.h.dl = x;
	RATregs.h.dh = y;
	RATregs.h.bh = 0;
	RATregs.h.ah = 2;
	int86(0x10,&RATregs,&RATregs);
}

void ROMBasic(void)
{
	pokeb(0x0000,0462,0x0000);
	int86(0x18,&RATregs,&RATregs);
}

void coldboot(void)
{
	pokeb(0x0000,0x0472,0x0000);
	int86(0x19,&RATregs,&RATregs);
}

void warmboot(void)
{
	pokeb(0x0000,0x0472,0x1234);
	int86(0x19,&RATregs,&RATregs);
}