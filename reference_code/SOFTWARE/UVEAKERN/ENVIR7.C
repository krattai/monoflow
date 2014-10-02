/* ENVIR7.C: Environment Program

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/

#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<dos.h>
#include"keyio.h"
#include"grfxform.h"
#include"button.h"
#include"rat.h"

	defform("50,50,10,test,70,70,10,test2,90,90,10,test3"); 
RATINFO rat;
typedef struct {
	char fld1[10];
	char fld2[10];
	char fld3[10];
} TESTER;
TESTER testform = {"test","test2","test3"};

/* function prototypes */
int checkinput(void);

void main(void)
{
	int key,
		formon = 0,
		buton = 1,
		f2 = 0,
		buttons,
		x,
		y;

	startup();						/* standard startup sequence */

	/* Button */
	defbutton(100,400,"End");
	defbutton(400,400,"Help");
	defplate(0,20,"File");
	defplate(59,20,"Form");
	defform("50,50,10,test,70,70,10,test2,90,90,10,test3"); 

	initrat(&rat);
	raton(&rat);

	do {
		key = checkinput();
		x = rat.horiz;
		y = rat.verti;
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,x,y)))
		{
			ratoff(&rat);
			if(formon)
			{
				removeform();
				formon = 0;
			}
			else
			{
				displayform();
				formon = 1;
			}
			raton(&rat);
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,x,y)))
		{
			ratoff(&rat);
			if(f2)
			{
				eraseform(0);
				f2 = 0;
			}
			else
			{
				showform(0);
				f2 = 1;
			}
			raton(&rat);
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,x,y)))
		{
			ratoff(&rat);
			if(buton)
			{
				buttonoff(1);
				buton = 0;
			}
			else
			{
				buttonon(1);
				buton = 1;
			}
			raton(&rat);
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,x,y)))
		{
			ratoff(&rat);
			if(buton)
			{
				buttonoff(1);
				buton = 0;
			}
			else
			{
				buttonon(1);
				buton = 1;
			}
			raton(&rat);
		}
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(0,x,y) || key == ESC));

	ratoff(&rat);

	shutdown();
}

int checkinput(void)
{
	int buts;

	buts = getratinput(&rat);
	if(keypress()) return(Get_key());
	return(buts);
}