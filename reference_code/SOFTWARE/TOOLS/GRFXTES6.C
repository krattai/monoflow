/* GRFXTES6.C: Tests possiblity of getimage()'ing and putimage()'ing to and
			from a file by redirecting the image pointer to a file pointer.

	Created by:		Kevin Rattai
	Date created:	May 23, 1993

*/

#include<graphics.h>
#include<stdlib.h>
#include<stdio.h>
#include<conio.h>

#define ARROW_SIZE 10

void draw_arrow(int x,int y);

int main(void)
{
	int gdriver=DETECT, gmode, errorcode;
	void *arrow;
	int x,y,maxx,maxy;
	unsigned size;

	/* auto-detect the graphics driver and mode */
	initgraph(&gdriver, &gmode, "");
	errorcode = graphresult(); /* check for any errors */
	if(errorcode != grOk)
	{
		printf("Graphics error: %s\n",grapherrormsg(errorcode));
		printf("Press any key to halt:");
		getch();
		exit(1);
	}
	maxx = getmaxx();
	maxy = getmaxy();
	x=0;
	y=maxy/2;

	draw_arrow(x,y);

	size=imagesize(x,y-ARROW_SIZE,x+(4*ARROW_SIZE),y+ARROW_SIZE);
	arrow=malloc(size);

	getimage(x,y-ARROW_SIZE,x+(4*ARROW_SIZE),y+ARROW_SIZE,arrow);
	putimage(x,y-ARROW_SIZE,arrow,XOR_PUT);
	y=0;

	while(!kbhit())
	{
/*		putimage(x,y-ARROW_SIZE,arrow,XOR_PUT); */
		x+=ARROW_SIZE;
		if(x>=maxx)
		{
			y+=ARROW_SIZE;
			x=0;
		}
		if(y>=maxy)
			y=0;
		putimage(x,y-ARROW_SIZE,arrow,XOR_PUT);
	}
	free(arrow);
	closegraph();
	return 0;
}

void draw_arrow(int x,int y)
{
	moveto(x,y);
	linerel(4*ARROW_SIZE,0);
	linerel(-2*ARROW_SIZE,-1*ARROW_SIZE);
	linerel(0,2*ARROW_SIZE);
	linerel(2*ARROW_SIZE,-1*ARROW_SIZE);
}