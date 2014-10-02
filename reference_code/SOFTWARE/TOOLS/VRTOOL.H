/* VRTOOL.H: VR Toolkit header file

	Mouse left 	- hand left
	Mouse right	- hand right
	Mouse up	- hand up
	Mouse down	- hand down
	Cursor up	- hand in
	Cursor down	- hand out

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/

#include<bios.h>
#include<stdio.h>
#include<process.h>
#include<graphics.h>
#include<conio.h>
#include<math.h>
#include"rat.h"
#include"keyio.h"

#define HANDCOLOR	2
#define NOHAND		0
#define MLEFT		50
#define MRIGHT		51
#define MUP			52
#define MDOWN		53

/* function prototypes */
int checkinput(void);
void turnon(void);
void turnoff(void);
/* void drawhand(int x, int y, int x1, int y1);
void erasehand(void);
void calcpos(int x, int y);
void movehand(int x, int y); */

float x=0.0,y=0.0,z=0.0;
float sx=0.0,sy=0.0;
float xa=0.0,ya=0.0,za=0.0;
float sxa=0.0,sya=0.0,sxb=0.0,syb=0.0;
float sxs=0.0,sys=0.0;
float d=1200.0;
double r1=5.68319;
double r2=6.28319;
double r3=5.79778;
double sr1=0.0,sr2=0.0,sr3=0.0;
double cr1=0.0,cr2=0.0,cr3=0.0;
float msx=0.0,msy=0.0,msz=-350.0;
int maxx=639,minx=0,maxy=199,miny=0;
float screen_x=639,screen_y=199;
float rx=0.0,ry=0.0;
int t1=0,t2=0;
int p1=0;

int array1[][3]={
	30,-30,30,30,-30,-30,-30,-30,-30,-30,-30,30,30,-30,30,
	30,30,-30,-30,30,-30,-30,-30,-30,30,-30,-30,30,30,-30,
	-30,30,-30,-30,30,30,-30,-30,30,-30,-30,-30,-30,30,-30,
	-30,30,30,30,30,30,30,-30,30,-30,-30,30,-30,30,30,
	30,30,30,30,30,-30,30,-30,-30,30,-30,30,30,30,30,
	-30,30,-30,30,30,-30,30,30,30,-30,30,30,-30,30,-30};

int array2[][3]={
	-30,-30,30,30,-30,30,0,23,4,-30,-30,30,
	-30,-30,30,30,-30,30,0,-30,-23,-30,-30,30,
	-30,-30,30,0,-30,-23,0,23,4,-30,-30,30,
	30,-30,30,0,23,4,0,-30,-23,30,-30,30};

int c0=0,c1=1,c2=2,c3=3,c4=4,c5=5,c6=6,c7=7,c8=8,c9=9,c10=10,
	c11=11,c12=12,c13=13,c14=14,c15=15,mode_flag=0;
float sx1,sy1,sx2,sy2;
float x_res,y_res;
RATINFO rat;

void keyboard(void);
void quit_pgm(void);
void calc_3d(void);
void rotation(void);
void windows(void);
void graphics_setup(void);
void newpos(void);

void main(void)
{
	int key,
		v,
		h;
	int oldv=0,oldh=0;
	int x1,y1,x2,y2;
	int x;

	turnon();
	graphics_setup();
	setcolor(c7);

	initrat(&rat);
	key = checkinput();
	h=rat.horiz;
	v=rat.verti;
	oldh=h;
	oldv=v;
	r1=0.0;
	r2=0.0;
	r3=0.0;
	msz=-150;
	newpos();
	while(!(key==ESC))
	{
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;
		if(oldh!=h)
		{
			if(oldh<h)
			{
				if(r1+0.1>6.28319)
					r1=0.0;
				else
					r1+=0.1;
			}
			else
			{
				if(r1-0.1<0.0)
					r1=6.28319;
				else
					r1-=0.1;
			}
			newpos();
		}
		if(oldv!=v)
		{
			if(oldv>v)
			{
				if(r3+0.1>6.28319)
					r3=0.0;
				else
					r3+=0.1;
			}
			else
			{
				if(r3-0.1<0.0)
					r3=6.28319;
				else
					r3-=0.1;
			}
			newpos();
		}
		resetrat(&rat);
		h=rat.horiz;
		v=rat.verti;
		oldh=h;
		oldv=v;
		switch(key)
		{
			case UP:	msz+=10;
						newpos();
						break;
			case DOWN:	msz-=10;
						newpos();
						break;
			case LEFT:	if(r2+0.1>6.28319)
							r2=0.0;
						else
							r2+=0.1;
						newpos();
						break;
			case RIGHT:	if(r2-0.1<0.0)
							r2=6.28319;
						else
							r2-=0.1;
						newpos();
						break;
/*			case PGUP:	hand[0]=x;
						hand[1]=y-17;
						hand[2]=x;
						hand[3]=y+8;
						hand[4]=x-10;
						hand[5]=y-12;
						hand[6]=x;
						hand[7]=y;
						hand[8]=x+10;
						hand[9]=y-12;
						hand[10]=x;
						hand[11]=y;
						drawhand(h,v,oldh,oldv);
						break;
			case PGDN:	hand[0]=x;
						hand[1]=y-17;
						hand[2]=x;
						hand[3]=y+8;
						hand[4]=x-10;
						hand[5]=y-12;
						hand[6]=x;
						hand[7]=y;
						hand[8]=x+10;
						hand[9]=y-12;
						hand[10]=x;
						hand[11]=y;
						drawhand(h,v,oldh,oldv);
						break; */
		}
	}
	ratoff();
	turnoff();
	quit_pgm();
}

void newpos(void)
{
	cleardevice();
	x=0.0;y=0.0;z=0.0;
	sx=0.0;sy=0.0;
	xa=0.0;ya=0.0;za=0.0;
	sxa=0.0;sya=0.0;sxb=0.0;syb=0.0;
	sxs=0.0;sys=0.0;
	sr1=0.0;sr2=0.0;sr3=0.0;
	cr1=0.0;cr2=0.0;cr3=0.0;
	msx=0.0;msy=0.0;
	rx=0.0;ry=0.0;
	t1=0;t2=0;
	p1=0;
	rotation();
/*	for(t2=1;t2<=6;t2++) */
	for(t2=1;t2<=4;t2++)
	{
		setlinestyle(USERBIT_LINE,0xffff,NORM_WIDTH);

		/* get starting position */
/*		x=array1[p1][0];
		y=array1[p1][1];
		z=array1[p1][2]; */
		x=array2[p1][0];
		y=array2[p1][1];
		z=array2[p1][2];
		calc_3d();
		windows();
		sxa=sx;sya=sy;

		/* finish displaying side */
/*		for(t1=1;t1<=4;t1++) */
		for(t1=1;t1<=3;t1++)
		{
			p1++;
/*			x=array1[p1][0];
			y=array1[p1][1];
			z=array1[p1][2]; */
			x=array2[p1][0];
			y=array2[p1][1];
			z=array2[p1][2];
			calc_3d();
			windows();
			sxs=sx;
			sys=sy;
			sxb=sx;
			syb=sy;
			moveto(sxa,sya);
			lineto(sxb,syb);
			sxa=sxs;
			sya=sys;
		}
		p1++;
	}
}

void rotation(void)
{
	sr1=sin(r1);
	sr2=sin(r2);
	sr3=sin(r3);
	cr1=cos(r1);
	cr2=cos(r2);
	cr3=cos(r3);
	return;
}

void calc_3d(void)
{
	/* rotate */
	x=(-1)*x;
	xa=cr1*x-sr1*z;
	za=sr1*x+cr1*z;
	x=cr2*xa+sr2*y;
	ya=cr2*y-sr2*xa;
	z=cr3*za-sr3*ya;
	y=sr3*za+cr3*ya;

	/* translate */
	x=x+msx;
	y=y+msy;
	z=z+msz;
	if(z==0) z=0.00000001;

	/* project */
	sx=d*x/z;
	sy=d*y/z;
	if(z==0.00000001) z=0.0;
	return;
}

void windows(void)
{
	sx=sx+399;
	sy=sy+299;
	rx=screen_x/799;
	ry=screen_y/599;
	sx=sx*rx;
	sy=sy*ry;
	return;
}

void keyboard(void)
{
	if(bioskey(1)==0)
		return;
	else
		quit_pgm();
}

void quit_pgm(void)
{
	cleardevice();
	restorecrtmode();
	exit(0);
}

void graphics_setup(void)
{
	int graphics_adapter,gr_mode;

	graphics_adapter=VGA;gr_mode=VGAHI;
	initgraph(&graphics_adapter,&gr_mode,"");
	x_res=640;
	y_res=480;
	mode_flag=1;
	maxx=639;
	minx=0;
	maxy=479;
	miny=0;
	screen_x=639;
	screen_y=479;
	setcolor(7);
	return;
}

RATINFO rat;
int graphdriver = VGA, graphmode = VGAHI;
struct text_info original;
int hand[18];

/* void main(void)
{
	int key,
		v,
		h;
	int oldv=0,oldh=0;
	int x1,y1,x2,y2;

	turnon();

	initrat(&rat);
	key = checkinput();
	h = rat.horiz;
	v = rat.verti;
	calcpos(h,v);
	oldh=h;
	oldv=v;
	drawhand(h,v,oldh,oldv);

	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;
		switch(key) {
			case UP:	erasehand();
						hand[1]++;
						hand[4]--;
						hand[6]++;
						hand[7]++;
						hand[12]--;
						hand[13]++;
						drawhand(h,v,oldh,oldv);
						break;
			case DOWN:	erasehand();
						hand[1]--;
						hand[4]++;
						hand[6]--;
						hand[7]--;
						hand[12]++;
						hand[13]--;
						drawhand(h,v,oldh,oldv);
						break; */
/*			case LEFT:	hand[0]=x;
						hand[1]=y-17;
						hand[2]=x;
						hand[3]=y+8;
						hand[4]=x-10;
						hand[5]=y-12;
						hand[6]=x;
						hand[7]=y;
						hand[8]=x+10;
						hand[9]=y-12;
						hand[10]=x;
						hand[11]=y;
						drawhand(h,v,oldh,oldv);
						break;
			case RIGHT:	hand[0]=x;
						hand[1]=y-17;
						hand[2]=x;
						hand[3]=y+8;
						hand[4]=x-10;
						hand[5]=y-12;
						hand[6]=x;
						hand[7]=y;
						hand[8]=x+10;
						hand[9]=y-12;
						hand[10]=x;
						hand[11]=y;
						drawhand(h,v,oldh,oldv);
						break;
			case PGUP:	hand[0]=x;
						hand[1]=y-17;
						hand[2]=x;
						hand[3]=y+8;
						hand[4]=x-10;
						hand[5]=y-12;
						hand[6]=x;
						hand[7]=y;
						hand[8]=x+10;
						hand[9]=y-12;
						hand[10]=x;
						hand[11]=y;
						drawhand(h,v,oldh,oldv);
						break;
			case PGDN:	hand[0]=x;
						hand[1]=y-17;
						hand[2]=x;
						hand[3]=y+8;
						hand[4]=x-10;
						hand[5]=y-12;
						hand[6]=x;
						hand[7]=y;
						hand[8]=x+10;
						hand[9]=y-12;
						hand[10]=x;
						hand[11]=y;
						drawhand(h,v,oldh,oldv);
						break; */
/*		}
		if(oldh!=h || oldv!=v)
		{
			erasehand();
			drawhand(h,v,oldh,oldv);
			oldh=h;
			oldv=v;
		}
	} while(!(key == ESC));

	ratoff();

	turnoff();
}

void drawhand(int x, int y, int x1, int y1)
{
	movehand(x-x1,y-y1);
	setcolor(HANDCOLOR);
	line(hand[0],hand[1],hand[3],hand[4]);
	line(hand[6],hand[7],hand[9],hand[10]);
	line(hand[12],hand[13],hand[15],hand[16]);
}

void movehand(int x, int y)
{
	hand[0]+=x;
	hand[1]+=y;
	hand[3]+=x;
	hand[4]+=y;
	hand[6]+=x;
	hand[7]+=y;
	hand[9]+=x;
	hand[10]+=y;
	hand[12]+=x;
	hand[13]+=y;
	hand[15]+=x;
	hand[16]+=y;
} */

void calcpos(int x, int y)
{
	hand[0]=x;
	hand[1]=y-17;
	hand[2]=0 /* z */;
	hand[3]=x;
	hand[4]=y+8;
	hand[5]=0 /* z */;
	hand[6]=x-10;
	hand[7]=y-12;
	hand[8]=0 /* z */;
	hand[9]=x;
	hand[10]=y;
	hand[11]=0 /* z */;
	hand[12]=x+10;
	hand[13]=y-12;
	hand[14]=0 /* z */;
	hand[15]=x;
	hand[16]=y;
	hand[17]=0 /* z */;
}

/* void erasehand(void)
{
	setcolor(NOHAND);
	line(hand[0],hand[1],hand[3],hand[4]);
	line(hand[6],hand[7],hand[9],hand[10]);
	line(hand[12],hand[13],hand[15],hand[16]);
}
*/
void turnon(void)
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
/*	gettextinfo(&original);
	initgraph(&graphdriver, &graphmode, ""); */
}

void turnoff(void)
{
/*	closegraph();

	textattr(original.attribute);
	window(original.winleft,
			original.wintop,
			original.winright,
			original.winbottom); */

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

int checkinput(void)
{
	int buts;

	buts = getratinput(&rat);
	if(keypress()) return(Get_key());
	return(buts);
}