/* GRFXFORM.C: Graphics Form module

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

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

/* globals */
int graphdriver = VGA, graphmode = VGAHI;
struct text_info original;


void changecolor(int color,int r,int g,int b)
{
	setrgbpalette(color,r,g,b);
	setcolor(color);
}

void putlogo(void)
{
	setcolor(CCSLLTBLUE);
	line(105,0,565,0);
	line(105,465,565,465);
	line(105,0,105,465);
	line(565,0,565,465);
	setfillstyle(SOLID_FILL,CCSLLTBLUE);
	floodfill(106,1,CCSLLTBLUE);

	setcolor(CCSLDKBLUE);
	line(125,25,545,25);
	line(125,445,545,445);
	line(125,25,125,445);
	line(545,25,545,445);
	setfillstyle(SOLID_FILL,CCSLDKBLUE);
	floodfill(126,26,CCSLDKBLUE);

	setcolor(CCSLLTBLUE);
	line(125,25,155,55);
	line(505,405,545,445);

	circle(230,130,105);
	setfillstyle(SOLID_FILL,CCSLLTBLUE);
	floodfill(230,150,CCSLLTBLUE);

	circle(440,340,105);
	floodfill(440,340,CCSLLTBLUE);

	setcolor(CCSLDKBLUE);
	circle(334,233,105);
	setfillstyle(SOLID_FILL,CCSLDKBLUE);
	floodfill(250,178,CCSLDKBLUE);
	floodfill(399,287,CCSLDKBLUE);

	setcolor(CCSLLTBLUE);
	circle(440,130,105);
	setfillstyle(SOLID_FILL,CCSLLTBLUE);
	floodfill(440,150,CCSLLTBLUE);

	circle(230,340,105);
	floodfill(230,340,CCSLLTBLUE);

	setcolor(CCSLDKBLUE);
	line(125,445,545,25);

	sleep(2);
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
	settextstyle(SANSSERIF, HORIZ_DIR, CCSLDKBLUE);
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
/*	putlogo();	*/					/* display logo */
/*	savelogo();
	removelogo();	*/				/* remove logo */
	placelogo();
	removelogo();
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

	/* setup fonts */
	rename("goth.chr","ccslchr1.fnt");
	rename("litt.chr","ccslchr2.fnt");
	rename("sans.chr","ccslchr3.fnt");
	rename("trip.chr","ccslchr4.fnt");

	/* setup graphics interface */
	rename("att.bgi","ccslgfx1.gri");
	rename("cga.bgi","ccslgfx2.gri");
	rename("egavga.bgi","ccslgfx3.gri");
	rename("herc.bgi","ccslgfx4.gri");
	rename("ibm8514.bgi","ccslgfx5.gri");
	rename("pc3270.bgi","ccslgfx6.gri");
}

void setupforms(void)
{
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

void savelogo(void)
{
/*	until end of page
	read pixel color
	save pixel color */

	int new_color,old_color;
	int x,y,count = -5;
	FILE *fp;
	char name[] = "logo2.pic";

	if(!(fp=fopen(name,"wb")))
	{
		cprintf("\nCannot open file.");
	}
	else
	{
		for(y=0;y<=480;y++)
		{
			for(x=0;x<=640;x++)
			{
				new_color = getpixel(x,y);
				if(count == -5)
				{
					old_color = new_color;
					count = 0;
				}
				if(old_color == new_color)
				{
					count++;
				}
				else
				{
					fwrite(&old_color,sizeof(int),1,fp);
					fwrite(&count,sizeof(int),1,fp);
					count = 1;
					old_color = new_color;
				}
			}
		}
		fwrite(&old_color,sizeof(int),1,fp);
		fwrite(&count,sizeof(int),1,fp);
		fclose(fp);
	}
}

void placelogo(void)
{
/*	until end of page
	read pixel color
	place pixel */

	int color;
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
			for(x=0;x<=640;x++)
			{
				if(count)
				{
					putpixel(x,y,color);
					count--;
				}
				else
				{
					fread(&color,sizeof(int),1,fp);
					fread(&count,sizeof(int),1,fp);
					putpixel(x,y,color);
					count--;
				}
			}
		}
		fclose(fp);
	}
}
