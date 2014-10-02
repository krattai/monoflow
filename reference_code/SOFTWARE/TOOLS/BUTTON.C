/* BUTTON.C: Button module

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

	Modification:	May 23, 1993
	Description:	Added function remove_but() to get rid of buttons that
						are no longer required.
	Modified by:	Kevin Rattai

	Modification:	May 24, 1993
	Description:	Cleaned and sped up removal of buttons by "turning button
					off" first and not printing text (not required).
	Modified by:	Kevin Rattai

*/

#include<stdio.h>
#include<dos.h>
#include<graphics.h>
#include"button.h"
#include"gfxform2.h"
#include"rat.h"

SCRNBUTTON scrnbuttons[30];

int uveabuttons[10] = {0,0,0,0,0,0,0,0,0,0};
int uveaplates[20] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
extern RATINFO rat;

void putbutton(SCRNBUTTON *but)
{
	int y;

	setcolor(UVEAWHITE);
	ratoff();
	for(y=but->top;y<=but->bot;y++) line(but->left,y,but->right,y);
	raton();

	setcolor(UVEADKBLUE);
	settextstyle(SANSSERIF, HORIZ_DIR, 2);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	ratoff();
	outtextxy(but->left + ((but->right - but->left)/2),
				but->top + ((but->bot - but->top)/2) - 2,
				but->msg);
	raton();
}

void buttonon(int x)
{
	ratoff();
	showbounds(UVEAGREY,UVEACHARCOAL,
				scrnbuttons[x].left,
				scrnbuttons[x].top,
				scrnbuttons[x].right,
				scrnbuttons[x].bot);
	raton();
}

void buttonoff(int x)
{
	ratoff();
	showbounds(UVEACHARCOAL,UVEABLACK,
				scrnbuttons[x].left,
				scrnbuttons[x].top,
				scrnbuttons[x].right,
				scrnbuttons[x].bot);
	raton();
}

void showbounds(int tlcolor, int brcolor,
				int left, int top, int right, int bot)
{
	setcolor(tlcolor);
	/* top */
	line(left-1,top,right+1,top);
	line(left-2,top-1,right+2,top-1);
	line(left-3,top-2,right+3,top-2);
	/* left */
	line(left-1,top,left-1,bot);
	line(left-2,top-1,left-2,bot+1);
	line(left-3,top-2,left-3,bot+2);

	setcolor(brcolor);
	/* bottom */
	line(left-1,bot+1,right+1,bot+1);
	line(left-2,bot+2,right+2,bot+2);
	line(left-3,bot+3,right+3,bot+3);
	/* right */
	line(right+1,top,right+1,bot);
	line(right+2,top-1,right+2,bot+1);
	line(right+3,top-2,right+3,bot+2);
}

void defbutton(int left, int top, char msg[])
{
	int x = 0,y;

/*		CAUTION:  For your own sanity, this function should be use like
				HEAP or STACK; the most important buttons that will be
				least likely to be removed should be put in first */

	while(uveabuttons[x]) x++;
	scrnbuttons[x].number = x+1;
	uveabuttons[x] = x+1;
	scrnbuttons[x].left = left;
	scrnbuttons[x].top = top;
	scrnbuttons[x].bot = scrnbuttons[x].top + 18;
	y = 0;
	while(y<=strlen(msg))
	{
		scrnbuttons[x].msg[y] = msg[y];
		y++;
	}
	scrnbuttons[x].right = scrnbuttons[x].left
						+ (strlen(scrnbuttons[x].msg) * 14);
	putbutton(&scrnbuttons[x]);
	buttonon(x);
}

void defplate(int left, int top, char msg[])
{
	int x = 0,y;

	while(uveaplates[x]) x++;
	scrnbuttons[x+10].number = x+1;
	uveaplates[x] = x+1;
	x+=10;
	scrnbuttons[x].left = left;
	scrnbuttons[x].top = top;
	scrnbuttons[x].bot = scrnbuttons[x].top + 17;
	y = 0;
	while(y<=strlen(msg))
	{
		scrnbuttons[x].msg[y] = msg[y];
		y++;
	}
	scrnbuttons[x].right = scrnbuttons[x].left
						+ ((strlen(scrnbuttons[x].msg) * 6) + 2);
	putplate(&scrnbuttons[x]);
}

void putplate(SCRNBUTTON *but)
{
	int y;

	setcolor(UVEAGREY);
	ratoff();
	for(y=but->top;y<=but->bot;y++) line(but->left,y,but->right,y);
	raton();
	setcolor(UVEAWHITE);
	y = but->left + ((but->right - but->left)*2/3);
	ratoff();
	line(but->left,but->top,y,but->top);
	y = but->top + ((but->bot - but->top)*2/3);
	line(but->left,but->top,but->left,y);
	setcolor(UVEACHARCOAL);
	y = but->left + ((but->right - but->left)*1/3);
	line(y,but->bot,but->right,but->bot);
	y = but->top + ((but->bot - but->top)*1/3);
	line(but->right,y,but->right,but->bot);
	raton();

	setcolor(UVEABLACK);
	settextstyle(SMALL_FONT, HORIZ_DIR, 0);
	setusercharsize(1,1,11,8);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	ratoff();
	outtextxy(but->left + ((but->right - but->left)/2),
				but->top + ((but->bot - but->top)/2) - 2,
				but->msg);
	raton();
}

int checkhit(int z, int x, int y)
{
	if(((x>=scrnbuttons[z].left)
		&& (x<=scrnbuttons[z].right))
		&& ((y>=scrnbuttons[z].top)
		&& (y<=scrnbuttons[z].bot)))
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

int platehit(int z, int x, int y)
{
	z+=10;
	if(((x>=scrnbuttons[z].left)
		&& (x<=scrnbuttons[z].right))
		&& ((y>=scrnbuttons[z].top)
		&& (y<=scrnbuttons[z].bot)))
	{
		return(1);
	}
	else
	{
		return(0);
	}
}

void clearplates(void)
{
	int x = 0,y;

	while(uveaplates[x])
	{
		remplate(&scrnbuttons[x+10]);
		uveaplates[x] = 0;
		scrnbuttons[x+10].number = 0;
		scrnbuttons[x+10].left = 0;
		scrnbuttons[x+10].top = 0;
		scrnbuttons[x+10].bot = 0;
		scrnbuttons[x+10].right = 0;
		x++;
	}
}

void remplate(SCRNBUTTON *but)
{
	int y;

	setcolor(UVEADKBLUE);
	ratoff();
	for(y=but->top;y<=but->bot;y++) line(but->left,y,but->right,y);
	raton();
	setcolor(UVEADKBLUE);
	y = but->left + ((but->right - but->left)*2/3);
	ratoff();
	line(but->left,but->top,y,but->top);
	y = but->top + ((but->bot - but->top)*2/3);
	line(but->left,but->top,but->left,y);
	setcolor(UVEADKBLUE);
	y = but->left + ((but->right - but->left)*1/3);
	line(y,but->bot,but->right,but->bot);
	y = but->top + ((but->bot - but->top)*1/3);
	line(but->right,y,but->right,but->bot);
	raton();
}

void remove_button(int num)
{
	int x = 0;

	/* This function was done this way so as not to disrupt code flow
		from calling program if a new button is added, it should replace
		buttons which have been removed first before appending */

	/* clear button from display */
	clearbutton(&scrnbuttons[num]);

	/* remove button from set */
	scrnbuttons[num].number = 0;
	uveabuttons[num] = 0;
	scrnbuttons[num].left = 0;
	scrnbuttons[num].top = 0;
	scrnbuttons[num].bot = 0;
	scrnbuttons[num].right = 0;
	while(x<=39)
	{
		scrnbuttons[num].msg[x] = NULL;
		x++;
	}
}

void clearbutton(SCRNBUTTON *but)
{
	int y;

	ratoff();
	showbounds(UVEADKBLUE,UVEADKBLUE,but->left,but->top,but->right,but->bot);
	raton();
	setcolor(UVEADKBLUE);
	ratoff();
	for(y=but->top;y<=but->bot;y++) line(but->left,y,but->right,y);
	raton();
}
