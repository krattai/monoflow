/* ENVIR8.C: Environment Program

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/

#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<dos.h>
#include<process.h>
#include"keyio.h"
#include"grfxform.h"
#include"button.h"
#include"rat.h"

RATINFO rat;
typedef struct {
	char fld1[10];
	char fld2[10];
	char fld3[10];
} TESTER;
TESTER testform = {"test","test2Yup","test3"};

/* function prototypes */
int checkinput(void);

void main(void)
{
	int key,
		formon = 0,
		buton = 1,
		f2 = 0,
		busy = 0,
		v,
		h,
		z;
	char test;
	int garb;

	startup();						/* standard startup sequence */

	/* Button */
	defbutton(100,450,"End");
	defbutton(400,450,"Help");
	defplate(0,25,"File");
	defplate(58,25,"Form");
    defplate(150,25,"DOS");
	defplate(350,25,"Display");
	defplate(450,25,"Clear");
	defplate(522,25,"Edit");
	defform("50,50,4,test,50,70,8,test2,50,90,6,test3"); 

	initrat(&rat);
	raton(&rat);

	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
			ratoff(&rat);
			if(formon)
			{
				if(f2) busy = 0;
				removeform();
				formon = 0;
			}
			else
			{
				if(f2) busy = 1;
				displayform();
				formon = 1;
			}
			raton(&rat);
		}
		if((rat.buttons == RBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
			garb = spawnle(P_WAIT,"c:\\command.com",NULL,NULL);
			if(garb == -1)
			{
				shutdown();
				perror("\nError");
				getch();
				exit(1);
			}
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
			if(f2 && !busy)
			{
				ratoff(&rat);
				showdata(0,0,testform.fld1);
				showdata(0,1,testform.fld2);
				showdata(0,2,testform.fld3);
				raton(&rat);
			}
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(4,h,v)))
		{
			if(f2 && !busy)
			{
				ratoff(&rat);
				cleanform(0,0,testform.fld1);
				cleanform(0,1,testform.fld2);
				cleanform(0,2,testform.fld3);
				raton(&rat);
			}
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(5,h,v)))
		{
			if(f2 && !busy)
			{
				ratoff(&rat);
				cleanform(0,0,testform.fld1);
				for(z=0;z<=10;z++) testform.fld1[z]=NULL;
				test=get_key();
				z=0;
				while(test != ENTER && z < 4)
				{
					testform.fld1[z]=test;
					showdata(0,0,testform.fld1);
					z++;
					test=get_key();
				}
				cleanform(0,1,testform.fld2);
				for(z=0;z<=10;z++) testform.fld2[z]=NULL;
				test=get_key();
				z=0;
				while(test != ENTER && z < 8)
				{
					testform.fld2[z]=test;
					showdata(0,1,testform.fld2);
					z++;
					test=get_key();
				}
				cleanform(0,2,testform.fld3);
				for(z=0;z<=10;z++) testform.fld3[z]=NULL;
				test=get_key();
				z=0;
				while(test != ENTER && z < 6)
				{
					testform.fld3[z]=test;
					showdata(0,2,testform.fld3);
					test=get_key();
					z++;
				}
				raton(&rat);
			}
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
			ratoff(&rat);
			if(f2)
			{
				eraseform(0,WITHOUT);
				f2 = 0;
			}
			else
			{
				showform(0,WITHOUT);
				f2 = 1;
			}
			raton(&rat);
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
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
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
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
			&& checkhit(0,h,v) || key == ESC));

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