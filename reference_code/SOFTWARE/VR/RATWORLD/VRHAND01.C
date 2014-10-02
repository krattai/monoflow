/* VRHAND01.C: Create 3D Hand for VR applications

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/

#include<graphics.h>
#include<conio.h>
#include<math.h>
#include"rat.h"
#include"keyio.h"

/* function prototypes */
int checkinput(void);
void turnon(void);
void turnoff(void);
void drawhand(int x, int y, int x1, int y1);
void erasehand(void);
void calcpos(int x, int y);
void movehand(int x, int y);


RATINFO rat;
int graphdriver = VGA, graphmode = VGAHI;
struct text_info original;
int hand[12];

void main(void)
{
	int key,
		v,
		h;
	int oldv=15,oldh=10;
	int x1,y1,x2,y2;

	turnon();

	initrat(&rat);
	raton(&rat);
	key = checkinput();
	h = rat.horiz;
	v = rat.verti;
	calcpos(h,v);
	oldh=h;
	oldv=v;
	drawhand(h,v,oldh,oldv);
	ratoff(&rat);

	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;
		switch(key) {
			case UP:	erasehand();
						hand[1]++;
						hand[3]--;
						hand[4]++;
						hand[5]++;
						hand[8]--;
						hand[9]++;
						drawhand(h,v,oldh,oldv);
						break;
			case DOWN:	erasehand();
						hand[1]--;
						hand[3]++;
						hand[4]--;
						hand[5]--;
						hand[8]++;
						hand[9]--;
						drawhand(h,v,oldh,oldv);
						break;
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
						break; */
		}
		if(oldh!=h || oldv!=v)
		{
			erasehand();
			drawhand(h,v,oldh,oldv);
			oldh=h;
			oldv=v;
		}
	} while(!(key == ESC));

	ratoff(&rat);

	turnoff();
}

void drawhand(int x, int y, int x1, int y1)
{
	movehand(x-x1,y-y1);
	setcolor(4);
	line(hand[0],hand[1],hand[2],hand[3]);
	line(hand[4],hand[5],hand[6],hand[7]);
	line(hand[8],hand[9],hand[10],hand[11]);
}

void movehand(int x, int y)
{
	hand[0]+=x;
	hand[1]+=y;
	hand[2]+=x;
	hand[3]+=y;
	hand[4]+=x;
	hand[5]+=y;
	hand[6]+=x;
	hand[7]+=y;
	hand[8]+=x;
	hand[9]+=y;
	hand[10]+=x;
	hand[11]+=y;
}

void calcpos(int x, int y)
{
	hand[0]=x;
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
}

void erasehand(void)
{
	setcolor(0);
	line(hand[0],hand[1],hand[2],hand[3]);
	line(hand[4],hand[5],hand[6],hand[7]);
	line(hand[8],hand[9],hand[10],hand[11]);
}

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
	gettextinfo(&original);
	initgraph(&graphdriver, &graphmode, "");
}

void turnoff(void)
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

int checkinput(void)
{
	int buts;

	buts = getratinput(&rat);
	if(keypress()) return(Get_key());
	return(buts);
}