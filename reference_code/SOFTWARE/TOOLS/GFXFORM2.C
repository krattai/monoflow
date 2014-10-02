/* GRFXFORM.C: Graphics Form module

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

	Modification:	May 22, 1993
	Description:	Updating Requests.
					Updated showform() and eraseform() to include displaying
						of field label on left side.
	Modified by:	Kevin Rattai

	Program update requests:
		When form is displayed, be sure to save an image of what the form is
			covering to allow the program the ability to re-display it
		Set up forms to be able to print field "titles" in the "standard"
			above or beside positions, as well as to manually position them
		Remove old displayform() and removeform(), they are obsolete

*/

#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<io.h>
#include<graphics.h>
#include<dos.h>
#include"grfxform.h"
#include"cminor.h"
#include"keyio.h"
#include"video.h"
#include"button.h"
#include"rat.h"

#define MAXFORMS	20

/* globals */
GRFXFORM grfxform[MAXFORMS];
int graphdriver = VGA, graphmode = VGAHI;
struct text_info original;
int forms[MAXFORMS]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


void changecolor(int color,int r,int g,int b)
{
	setrgbpalette(color,r,g,b);
	setcolor(color);
}

void removelogo(void)
{
	setfillstyle(SOLID_FILL,CCSLBLACK);
	floodfill(105,0,CCSLBLACK);
}

void putbackground(void)
{
	int x;

	setcolor(CCSLLTBLUE);
	for(x=0;x<=20;x++) line(0,x,639,x);

	setcolor(CCSLDKBLUE);
	line(0,21,639,21);
	line(0,479,639,479);
	line(0,21,0,479);
	line(639,21,639,479);

	setfillstyle(SOLID_FILL,CCSLDKBLUE);
	floodfill(1,22,CCSLDKBLUE);

	setcolor(CCSLDKBLUE);
	settextstyle(SANSSERIF, HORIZ_DIR, 2);
	settextjustify(CENTER_TEXT,CENTER_TEXT);
	outtextxy(getmaxx()/2,8,"Command Computer Services Ltd.");
}

void setccslpalette(void)
{
	changecolor(CCSLBLACK,0,0,0);			/* 0 = black */
	changecolor(CCSLLTBLUE,1,21,32);		/* 1 = light blue */
	changecolor(CCSLDKBLUE,0,0,18);			/* 2 = dark blue */
	changecolor(CCSLGREY,40,40,40);			/* 3 = grey */
	changecolor(CCSLWHITE,58,58,58);		/* 4 = white */
	changecolor(CCSLCHARCOAL,25,25,25);		/* 5 = charcoal */
}

void startup(void)
{
	/* setup fonts */
	rename("ccslchr1.fnt","goth.chr");
	rename("ccslchr2.fnt","litt.chr");
	rename("ccslchr3.fnt","sans.chr");
	rename("ccslchr4.fnt","trip.chr");

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
	putbackground();				/* display CCSL "wallpaper" */
}

void shutdown(void)
{
	closegraph();

	textattr(original.attribute);
	window(original.winleft,
			original.wintop,
			original.winright,
			original.winbottom);

	/* shutdown fonts */
	rename("goth.chr","ccslchr1.fnt");
	rename("litt.chr","ccslchr2.fnt");
	rename("sans.chr","ccslchr3.fnt");
	rename("trip.chr","ccslchr4.fnt");

	/* shutdown graphics interface */
	rename("att.bgi","ccslgfx1.gri");
	rename("cga.bgi","ccslgfx2.gri");
	rename("egavga.bgi","ccslgfx3.gri");
	rename("herc.bgi","ccslgfx4.gri");
	rename("ibm8514.bgi","ccslgfx5.gri");
	rename("pc3270.bgi","ccslgfx6.gri");
}

void displayform(void)
{
	int x;

	setcolor(CCSLGREY);
	for(x=61;x<=78;x++) line(5,x,632,x);
	setcolor(CCSLBLACK);
	line(5,x,634,x);
	line(6,x+1,634,x+1);
	line(633,x-16,633,x-1);
	line(634,x-16,634,x-1);
	setcolor(CCSLGREY);
	for(x=81;x<=98;x++) line(5,x,632,x);
	setcolor(CCSLBLACK);
	line(5,x,634,x);
	line(6,x+1,634,x+1);
	line(633,x-16,633,x-1);
	line(634,x-16,634,x-1);
	setcolor(CCSLWHITE);
	for(x=101;x<=118;x++) line(5,x,632,x);
	setcolor(CCSLBLACK);
	line(5,x,634,x);
	line(6,x+1,634,x+1);
	line(633,x-16,633,x-1);
	line(634,x-16,634,x-1);
	setcolor(CCSLGREY);
	for(x=121;x<=138;x++) line(5,x,632,x);
	setcolor(CCSLBLACK);
	line(5,x,634,x);
	line(6,x+1,634,x+1);
	line(633,x-16,633,x-1);
	line(634,x-16,634,x-1);
	setcolor(CCSLGREY);
	for(x=141;x<=158;x++) line(5,x,632,x);
	setcolor(CCSLBLACK);
	line(5,x,634,x);
	line(6,x+1,634,x+1);
	line(633,x-16,633,x-1);
	line(634,x-16,634,x-1);

	setcolor(CCSLGREY);
	for(x=261;x<=278;x++) line(5,x,632,x);
	setcolor(CCSLBLACK);
	line(5,x,634,x);
	line(6,x+1,634,x+1);
	line(633,x-16,633,x-1);
	line(634,x-16,634,x-1);
	setcolor(CCSLGREY);
	for(x=281;x<=298;x++) line(5,x,632,x);
	setcolor(CCSLBLACK);
	line(5,x,634,x);
	line(6,x+1,634,x+1);
	line(633,x-16,633,x-1);
	line(634,x-16,634,x-1);
	setcolor(CCSLWHITE);
	for(x=301;x<=318;x++) line(5,x,632,x);
	setcolor(CCSLBLACK);
	line(5,x,634,x);
	line(6,x+1,634,x+1);
	line(633,x-16,633,x-1);
	line(634,x-16,634,x-1);
	setcolor(CCSLGREY);
	for(x=321;x<=338;x++) line(5,x,632,x);
	setcolor(CCSLBLACK);
	line(5,x,634,x);
	line(6,x+1,634,x+1);
	line(633,x-16,633,x-1);
	line(634,x-16,634,x-1);
	setcolor(CCSLGREY);
	for(x=341;x<=358;x++) line(5,x,632,x);
	setcolor(CCSLBLACK);
	line(5,x,634,x);
	line(6,x+1,634,x+1);
	line(633,x-16,633,x-1);
	line(634,x-16,634,x-1);
}

void removeform(void)
{
	int x;

	setcolor(CCSLDKBLUE);
	for(x=61;x<=160;x++) line(0,x,639,x);
	for(x=261;x<=360;x++) line(0,x,639,x);
}

void putlogo(void)
{
	int color,go=1;
	int x,y,count;
	FILE *fp;
	char name[] = "logo2.pic";

	if(!(fp=fopen(name,"rb")))
	{
		cprintf("\nCannot open file.");
	}
	else
	{
		fread(&color,sizeof(int),1,fp);
		fread(&count,sizeof(int),1,fp);
		for(y=0;y<=480;y++)
		{
			if(keypress()) go=0;
			if(go)
			{
				for(x=0;x<=640;x++)
				{
					if(count)
					{
						if(color) putpixel(x,y,color);
						count--;
					}
					else
					{
						fread(&color,sizeof(int),1,fp);
						fread(&count,sizeof(int),1,fp);
						if(color) putpixel(x,y,color);
						count--;
					}
				}
			}
		}
		fclose(fp);
	}
	if(go) sleep(2);
	removelogo();
}

int defform(char form_string[])
{
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
	int x,fld;

	for(fld=0;fld<=grfxform[num].number;fld++)
	{
		setcolor(CCSLWHITE);
		for(x=grfxform[num].fld[fld].top;x<=grfxform[num].fld[fld].bot;x++)
			line(grfxform[num].fld[fld].left,x,
											grfxform[num].fld[fld].right,x);
		setcolor(CCSLBLACK);
		line(grfxform[num].fld[fld].left,x,
										grfxform[num].fld[fld].right+2,x);
		line(grfxform[num].fld[fld].left+1,x+1,
										grfxform[num].fld[fld].right+2,x+1);
		line(grfxform[num].fld[fld].right+1,x-16,
										grfxform[num].fld[fld].right+1,x-1);
		line(grfxform[num].fld[fld].right+2,x-16,
										grfxform[num].fld[fld].right+2,x-1);
	}
	setcolor(CCSLGREY);
	settextstyle(SANSSERIF, HORIZ_DIR, 2);
	settextjustify(RIGHT_TEXT,CENTER_TEXT);
	if(label == WITHOUT)
	{
	}
	if(label == LLEFT)
	{
		for(fld=0;fld<=grfxform[num].number;fld++)
		{
			x = grfxform[num].fld[fld].left - 5;
			outtextxy(x,grfxform[num].fld[fld].top+7,
									grfxform[num].fld[fld].label);
		}
	}
	if(label == LABOVE)
	{
	}
	return(0);
}

int eraseform(int num, int label)
{
	int x,fld;

	setcolor(CCSLDKBLUE);
	for(fld=0;fld<=grfxform[num].number;fld++)
	{
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
	}
	setcolor(CCSLDKBLUE);
	settextstyle(SANSSERIF, HORIZ_DIR, 2);
	settextjustify(RIGHT_TEXT,CENTER_TEXT);
	if(label == WITHOUT)
	{
	}
	if(label == LLEFT)
	{
		for(fld=0;fld<=grfxform[num].number;fld++)
		{
			x = grfxform[num].fld[fld].left - 5;
			outtextxy(x,grfxform[num].fld[fld].top+7,
									grfxform[num].fld[fld].label);
		}
	}
	if(label == LABOVE)
	{
	}
	return(0);
}

int showdata(int fnum, int fld, char data[])
{
	int len = 0,
		x = 0,
		pos = 0;
	char temp[1];

	setcolor(CCSLBLACK);
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
	return(0);
}

int cleanform(int fnum, int fld, char data[])
{
	int len = 0,
		x = 0,
		pos = 0;
	char temp[1];

	setcolor(CCSLWHITE);
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
