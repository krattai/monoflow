/* GFXFORM3.C: Graphics Form module

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

	Modification:	May 22, 1993
	Description:	Updating Requests.
					Updated showform() and eraseform() to include displaying
						of field label on left side.
					Updated this new version of GFXFORM3.C based on its
						predecessor GFXFORM2.C.
	Modified by:	Kevin Rattai

	Modification:	June 2, 1993
	Description:	Updated Requests.
	Modified by:	Kevin Rattai

	Modification:	June 7, 1993
	Description:	Added new fonts.
	Modified by:	Kevin Rattai

	Modification:	June 9, 1993
	Description:	Removed funtions displayform() and removeform().  No
						longer required.
    				Updated showform() and eraseform() to pass RATINFO.
	Modified by:	Kevin Rattai

	Modification:	September 26, 1993
	Description:	Added functions sprdsht() and remove sprdsht().
					Added remarks to functions.
	Modified by:	Kevin Rattai

    Modification:	Octoboer 20, 1993
    Description:	Removed unneeded code from putbackground().
    Modified by:	Kevin Rattai

	Program update requests:
		When form is displayed, be sure to save an image of what the form is
			covering to allow the program the ability to re-display it
		Set up forms to be able to print field "titles" in the "standard"
			above or beside positions, as well as to manually position them
		Remove old displayform() and removeform(), they are obsolete
		Create a function that will allow user to answer a question in a
			graphic box with "bullet" holes to change information.  

*/

#include<alloc.h>
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<io.h>
#include<graphics.h>
#include<dos.h>
#include"gfxform2.h"
#include"cminor.h"
#include"keyio.h"
#include"video.h"
#include"button.h"
#include"rat.h"
#include"pxengine.h"

#define MAXFORMS	20

/* globals */
GRFXFORM grfxform[MAXFORMS];
int graphdriver = VGA, graphmode = VGAHI;
struct text_info original;
int forms[MAXFORMS]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
extern RATINFO rat;

void changecolor(int color,int r,int g,int b)
{
	/* this function will change the current palette to
		display uvea colors */

	setrgbpalette(color,r,g,b);
	setcolor(color);
}

void removelogo(void)
{
	/* this function removes the uvea logo from the screen */

	setfillstyle(SOLID_FILL,UVEABLACK);
	floodfill(105,19,UVEABLACK);
}

void putbackground(void)
{
	/* this function displays the application "desktop" */

	int x;

	setcolor(UVEALTBLUE);
	for(x=0;x<=20;x++) line(0,x,639,x);

	setfillstyle(SOLID_FILL,UVEADKBLUE);
	floodfill(1,22,UVEALTBLUE);

	setcolor(UVEADKBLUE);
	settextstyle(SANSSERIF, HORIZ_DIR, 2);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	outtextxy(getmaxx()/2,8,"Uvea Information Services");
}

void setccslpalette(void)
{
	/* this function sets the colors for the UVEA palette */

	changecolor(UVEABLACK,0,0,0);			/* 0 = black */
	changecolor(UVEALTBLUE,1,21,32);		/* 1 = light blue */
	changecolor(UVEADKBLUE,0,0,18);			/* 2 = dark blue */
	changecolor(UVEAGREY,40,40,40);			/* 3 = grey */
	changecolor(UVEAWHITE,58,58,58);		/* 4 = white */
	changecolor(UVEACHARCOAL,25,25,25);		/* 5 = charcoal */
}

void startup(void)
{
	/* this function will set up the system for the UVEA application */

	/* setup fonts */
	rename("ccslchr0.fnt","tscr.chr");
	rename("ccslchr1.fnt","goth.chr");
	rename("ccslchr2.fnt","litt.chr");
	rename("ccslchr3.fnt","sans.chr");
	rename("ccslchr4.fnt","trip.chr");
	rename("ccslchr5.fnt","bold.chr");
	rename("ccslchr6.fnt","euro.chr");
	rename("ccslchr7.fnt","lcom.chr");
	rename("ccslchr8.fnt","scri.chr");
	rename("ccslchr9.fnt","simp.chr");

	/* setup graphics interface */
	rename("ccslgfx1.gri","att.bgi");
	rename("ccslgfx2.gri","cga.bgi");
	rename("ccslgfx3.gri","egavga.bgi");
	rename("ccslgfx4.gri","herc.bgi");
	rename("ccslgfx5.gri","ibm8514.bgi");
	rename("ccslgfx6.gri","pc3270.bgi");

	/* Set up graphics */
	gettextinfo(&original);
	initgraph(&graphdriver, &graphmode, "");
	setccslpalette();				/* set palette */
	putlogo();						/* display logo */
	putbackground();				/* display UVEA "wallpaper" */
	initrat(&rat);
	raton();
    PXInit();
}

void shutdown(void)
{
	/* this function restores the system upon application exit */

	ratoff();
	closegraph();

	textattr(original.attribute);
	window(original.winleft,
			original.wintop,
			original.winright,
			original.winbottom);

	/* shutdown fonts */
	rename("tscr.chr","ccslchr0.fnt");
	rename("goth.chr","ccslchr1.fnt");
	rename("litt.chr","ccslchr2.fnt");
	rename("sans.chr","ccslchr3.fnt");
	rename("trip.chr","ccslchr4.fnt");
	rename("bold.chr","ccslchr5.fnt");
	rename("euro.chr","ccslchr6.fnt");
	rename("lcom.chr","ccslchr7.fnt");
	rename("scri.chr","ccslchr8.fnt");
	rename("simp.chr","ccslchr9.fnt");

	/* shutdown graphics interface */
	rename("att.bgi","ccslgfx1.gri");
	rename("cga.bgi","ccslgfx2.gri");
	rename("egavga.bgi","ccslgfx3.gri");
	rename("herc.bgi","ccslgfx4.gri");
	rename("ibm8514.bgi","ccslgfx5.gri");
	rename("pc3270.bgi","ccslgfx6.gri");
    PXExit();
}

void putlogo(void)
{
	/* this function display the uvea logo on the screen */

	unsigned size;
	void far *ptr[3];
	FILE *fptr;
	int color;
	int x,y,count;

	/* logo based on these dimensions: 105,0,565,465 */
	size=imagesize(105,0,565,(465+1)/3);
	fptr=fopen("LOGO3.PIC","rb");
	restore_screen(fptr,size);
	fclose(fptr);

	/* wait */
	sleep(2);

    removelogo();
}

int defform(char form_string[])
{
	/* this function allows for programmer defined forms */

	char	cx[3] = "",
			cy[3] = "",
			lng[3] = "",
			*form,
			char_val;
	int		x,
			y,
			fld,
			formnum,
			where,
			pos,
			form_condition = 0;

	y = 0;
	for(x=0;x<=MAXFORMS;x++)
	{
		if(y==0 && forms[x]==0)
		{
			forms[x]=1;
			y=x+1;
		}
	}
	x = y-1;
	fld = 0;
	form = form_string;
	where = 0;
	pos = 0;
	while(*form != NULL)
	{
		if(*form == ',')
		{
			switch(where)
			{
				case 0:	grfxform[x].fld[fld].left = atoi(cx);
						where++;
						break;
				case 1:	grfxform[x].fld[fld].top = atoi(cy);
						grfxform[x].fld[fld].bot =
								grfxform[x].fld[fld].top + 18;
						where++;
						break;
				case 2: grfxform[x].fld[fld].right =
								grfxform[x].fld[fld].left +
													(atoi(lng) * 14);
						where++;
						break;
				case 3: where = 0;
						for(pos = 0; pos <= 3; pos++) cx[pos] = NULL;
						for(pos = 0; pos <= 3; pos++) cy[pos] = NULL;
						for(pos = 0; pos <= 3; pos++) lng[pos] = NULL;
						fld++;
						break;
			}
			pos = 0;
		}
		else
		{
			switch(where)
			{
				case 0:	cx[pos] = *form;
						break;
				case 1:	cy[pos] = *form;
						break;
				case 2: lng[pos] = *form;
						break;
				case 3:	grfxform[x].fld[fld].label[pos] = *form;
						break;
			}
			pos++;
		}
		form++;
	}
	grfxform[x].fld[fld].left = atoi(cx);
	grfxform[x].fld[fld].top = atoi(cy);
	grfxform[x].fld[fld].bot = grfxform[x].fld[fld].top + 18;
	grfxform[x].fld[fld].right = grfxform[x].fld[fld].left +
													(atoi(lng) * 14);
	grfxform[x].number = fld;
	return(form_condition);
}

int showform(int num, int label)
{
	/* this function displays the programmer defined forms */

	int x,fld;

	for(fld=0;fld<=grfxform[num].number;fld++)
	{
		setcolor(UVEAWHITE);
		ratoff();
		for(x=grfxform[num].fld[fld].top;x<=grfxform[num].fld[fld].bot;x++)
			line(grfxform[num].fld[fld].left,x,
											grfxform[num].fld[fld].right,x);
		raton();
		setcolor(UVEABLACK);
		ratoff();
		line(grfxform[num].fld[fld].left,x,
										grfxform[num].fld[fld].right+2,x);
		line(grfxform[num].fld[fld].left+1,x+1,
										grfxform[num].fld[fld].right+2,x+1);
		line(grfxform[num].fld[fld].right+1,x-16,
										grfxform[num].fld[fld].right+1,x-1);
		line(grfxform[num].fld[fld].right+2,x-16,
										grfxform[num].fld[fld].right+2,x-1);
		raton();
	}
	setcolor(UVEAGREY);
	settextstyle(SANSSERIF, HORIZ_DIR, 2);
	if(label == WITHOUT)
	{
	}
	if(label == LLEFT)
	{
		settextjustify(RIGHT_TEXT,CENTER_TEXT);
		ratoff();
		for(fld=0;fld<=grfxform[num].number;fld++)
		{
			x = grfxform[num].fld[fld].left - 5;
			outtextxy(x,grfxform[num].fld[fld].top+7,
									grfxform[num].fld[fld].label);
		}
		raton();
	}
	if(label == LABOVE)
	{
		settextjustify(LEFT_TEXT,CENTER_TEXT);
		ratoff();
		for(fld=0;fld<=grfxform[num].number;fld++)
		{
			outtextxy(grfxform[num].fld[fld].left,
						grfxform[num].fld[fld].top+7-20,
						grfxform[num].fld[fld].label);
		}
		raton();
	}
	return(0);
}

int eraseform(int num, int label)
{
	/* this function removes a form from the screen */

	int x,fld;

	setcolor(UVEADKBLUE);
	for(fld=0;fld<=grfxform[num].number;fld++)
	{
		ratoff();
		for(x=grfxform[num].fld[fld].top;x<=grfxform[num].fld[fld].bot;x++)
			line(grfxform[num].fld[fld].left,x,
											grfxform[num].fld[fld].right,x);
		line(grfxform[num].fld[fld].left,x,
										grfxform[num].fld[fld].right+2,x);
		line(grfxform[num].fld[fld].left+1,x+1,
										grfxform[num].fld[fld].right+2,x+1);
		line(grfxform[num].fld[fld].right+1,x-16,
										grfxform[num].fld[fld].right+1,x-1);
		line(grfxform[num].fld[fld].right+2,x-16,
										grfxform[num].fld[fld].right+2,x-1);
		raton();
	}
	setcolor(UVEADKBLUE);
	settextstyle(SANSSERIF, HORIZ_DIR, 2);
	if(label == WITHOUT)
	{
	}
	if(label == LLEFT)
	{
		settextjustify(RIGHT_TEXT,CENTER_TEXT);
		ratoff();
		for(fld=0;fld<=grfxform[num].number;fld++)
		{
			x = grfxform[num].fld[fld].left - 5;
			outtextxy(x,grfxform[num].fld[fld].top+7,
									grfxform[num].fld[fld].label);
		}
		raton();
	}
	if(label == LABOVE)
	{
		settextjustify(LEFT_TEXT,CENTER_TEXT);
		ratoff();
		for(fld=0;fld<=grfxform[num].number;fld++)
		{
			outtextxy(grfxform[num].fld[fld].left,
						grfxform[num].fld[fld].top+7-20,
						grfxform[num].fld[fld].label);
		}
		raton();
	}
	return(0);
}

int showdata(int fnum, int fld, char data[])
{
	/* this function displays data in a displayed form */

	int len = 0,
		x = 0,
		pos = 0;
	char temp[1];

	setcolor(UVEABLACK);
	settextstyle(SANSSERIF, HORIZ_DIR, 2);
	settextjustify(LEFT_TEXT,CENTER_TEXT);

	temp[1]=NULL;
	len = (grfxform[fnum].fld[fld].right - grfxform[fnum].fld[fld].left)
																	/ 14;
	x = grfxform[fnum].fld[fld].left+5;

	for(pos=0;pos<len;pos++)
	{
		temp[0] = data[pos];
		outtextxy(x,grfxform[fnum].fld[fld].top+7,temp);
		x+=12;
	}
	return(0);
}

int getdata(int fnum, char data[])
{
	/* this function will get information from a displayed form */

	return(0);
}

int cleanform(int fnum, int fld, char data[])
{
	/* this funtion clears information from a displayed form */

	int len = 0,
		x = 0,
		pos = 0;
	char temp[1];

	setcolor(UVEAWHITE);
	settextstyle(SANSSERIF, HORIZ_DIR, 2);
	settextjustify(LEFT_TEXT,CENTER_TEXT);

	temp[1]=NULL;
	len = (grfxform[fnum].fld[fld].right - grfxform[fnum].fld[fld].left)
																	/ 14;
	x = grfxform[fnum].fld[fld].left+5;

	for(pos=0;pos<len;pos++)
	{
		temp[0] = data[pos];
		outtextxy(x,grfxform[fnum].fld[fld].top+7,temp);
		x+=12;
	}
	return(0);
}

void restore_screen(FILE *fptr, unsigned size)
{
	/* this function will display actual logo and then restore screen */

	int ystart=19,yend,yincr,x;
	void far *ptr[3];

	yincr=(465+1)/3;
	yend=yincr+ystart;

	for(x=0;x<=2;x++)
	{
		if((ptr[x] = farmalloc(size)) == NULL)
		{
			closegraph();
			printf("Error: not enough heap size in restore_screen().\n");
			exit(1);
		}
		fread(ptr[x],size,1,fptr);
		putimage(105,ystart,ptr[x],COPY_PUT);
		ystart=yend+1;
		yend+=yincr+1;
		farfree(ptr[x]);
	}
}

int sprdsht(int fnum, int depth)
{
	/* this funtion displays a form in a spreadsheet like manner */

	int x,
		y,
		orgtop,
		orgbot;

	showform(fnum,LABOVE);
	orgtop=grfxform[fnum].fld[0].top;
	orgbot=grfxform[fnum].fld[0].bot;
	for(x=1;x<depth;x++)
	{
		for(y=0;y<=grfxform[fnum].number;y++)
		{
			grfxform[fnum].fld[y].top+=20;
			grfxform[fnum].fld[y].bot+=20;
		}
		showform(fnum,WITHOUT);
	}
	for(y=0;y<=grfxform[fnum].number;y++)
	{
		grfxform[fnum].fld[y].top=orgtop;
		grfxform[fnum].fld[y].bot=orgbot;
	}
	return(0);
}

int removesprdsht(int fnum, int depth)
{
	/* this funtion removes a spreadsheet form */

	int x,
		y,
		orgtop,
		orgbot;

	eraseform(fnum,LABOVE);
	orgtop=grfxform[fnum].fld[0].top;
	orgbot=grfxform[fnum].fld[0].bot;
	for(x=1;x<depth;x++)
	{
		for(y=0;y<=grfxform[fnum].number;y++)
		{
			grfxform[fnum].fld[y].top+=20;
			grfxform[fnum].fld[y].bot+=20;
		}
		eraseform(fnum,WITHOUT);
	}
	for(y=0;y<=grfxform[fnum].number;y++)
	{
		grfxform[fnum].fld[y].top=orgtop;
		grfxform[fnum].fld[y].bot=orgbot;
	}
	return(0);
}