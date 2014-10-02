/* FARM2.C: Farm Records One Prototype

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

	Modification:	September 26, 1993
	Description:	Added Procedures for Inventory - Real Estate Module
	Modified by:	Kevin Rattai

	Modification:	September 30, 1993
	Descrition:		Added Function defineforms()
					Added Procedures for Inventory - A/R & A/P
					Updated requests
	Modified by:	Kevin Rattai

    Modification:	October 8, 1993
    Description:	Added Procedures for Inventory - Personal
    Modified by:	Kevin Rattai

    Modification:	October 9, 1993
    Description:	Added Function definetables()
	Modified by:	Kevin Rattai

	Modification:	October 9, 1993
	Description:	Added Procedures for Inventory - Machinery
	Modified by:	Anna Laterza

    Modification:	October 11, 1993
	Description:	Added Procedures for Expenses - Special Bulk Accounts
    				Updated requests
                    Added function description remarks
    Modified by:	Kevin Rattai

    Modification:	October 12, 1993
    Description:	Added Procedures for Expenses - Car & Truck
    Modified by:	Kevin Rattai

    Modification:	October 13, 1993
    Description:	Added Procedures for Inventory - Grain and Feed
    Modified by:	Derek LaPorte

    Modification:	October 13, 1993
    Description:	Added Procedures for Expenses - Tractor, Combine, & Swathr
    Modified by:	Kevin Rattai

    Modification:	October 21, 1993
    Description:	Added convertit() to ccslmath.h & *.c
    Modified by:	Scott Dillabough

    Modification:	October 21, 1993
    Description:	Added Procedures for Expenses - Vehicles/Equip Othr
    Modified by:	Kevin Rattai

	program update requests:
	In Sub-Modules:  If flagged, paste plates on loop entry
						- therefore, not drawn if exit
    Create a function to take care of ALL checks to buttons and plates
    	ie. check_input() - returns PLATE1 hit

*/

#include<stdio.h>
#include<conio.h>
#include<graphics.h>
#include<dos.h>
#include"keyio.h"
#include"grfxform.h"
#include"button.h"
#include"rat.h"
#include"ccslmath.h"

RATINFO rat;

/* function prototypes */
void mainbuttons(void);
void mainplates(void);
void extrabutts(void);
void remextrabut(void);
int inventory(void);
void invplates(void);
int expenses(void);
void expplates(void);
int vehicles(void);
void vehplates(void);
int livestock(void);
void livplates(void);
int expother(void);
void othplates(void);
int receipts(void);
void recplates(void);
int summaries(void);
void sumplates(void);
int earnings(void);
void earplates(void);
int physical(void);
void phyplates(void);
int livestock1(void);
void liv1plates(void);
int land(void);
void lanplates(void);
int labour(void);
void labplates(void);
int enterprise(void);
void entplates(void);
void formplates(void);
int realestate(void);
void realplates(void);
int ownedreal(void);
int ownbare(void);
int ownimp(void);
int rentreal(void);
int rentbare(void);
int rentimp(void);
int invfarmpers(void);
int invfarmar(void);
int invfarmarLIS(void);
int invfar(void);
int invfap(void);
int invpersar(void);
int invpar(void);
int invpap(void);
int invhouse(void);
int invmachinery(void);
void rentplates(void);
void invfarmarplates(void);
void invpersapplates(void);
void LISplates(void);
void machineplates(void);
int expspecial(void);
void spclplate(void);
int expcartruck(void);
int invgrainfeed(void);
int expeqoth(void);
int expbldg(void);
int expcrop(void);
int definetables(void);
void defineforms(void);
int checkinput(void);
void help(int value);

/* main funtion - sets up environment on start and shuts down system */
void main(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	startup();						/* standard startup sequence */
    definetables();
	defineforms();
	mainbuttons();
	mainplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
			clearplates();
			status = inventory();
			mainplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
			clearplates();
			status = expenses();
			mainplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
			clearplates();
			status = receipts();
			mainplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(4,h,v)))
		{
			clearplates();
			status = summaries();
			mainplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(5,h,v)))
		{
			clearplates();
			status = physical();
			mainplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(6,h,v)))
		{
			clearplates();
			status = enterprise();
			mainplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(7,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if(status == 2) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(0,h,v) || key == ESC));

	shutdown();						/* standard shutdown sequence */
}

/* inventory module: access all inventory functions */
int inventory(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	invplates();
	extrabutts();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
			clearplates();
			status = realestate();
			invplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
			clearplates();
			status = invmachinery();
			invplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
        	clearplates();
            status = invgrainfeed();
            invplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(4,h,v)))
		{
			clearplates();
			status = invfarmpers();
			invplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(5,h,v)))
		{
        	clearplates();
            status = invhouse();
            invplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(6,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	remextrabut();
	return(status);
}

/* expenses module: accesses all expense modules */
int expenses(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	expplates();
	extrabutts();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
			clearplates();
			status = expspecial();
			expplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
			clearplates();
			status = vehicles();
			expplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
        	clearplates();
            status = expbldg();
            expplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
        	clearplates();
            status = expcrop();
            expplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(4,h,v)))
		{
			clearplates();
			status = livestock();
			expplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(5,h,v)))
		{
			clearplates();
			status = expother();
			expplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(6,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(7,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	remextrabut();
	return(status);
}

/* vehicles module: accesses vehicle expense modules */
int vehicles(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	vehplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
        	clearplates();
            status = expcartruck();
            vehplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
        	clearplates();
            status = expcartruck();
            vehplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
        	clearplates();
        	status = expcartruck();
            vehplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
        	clearplates();
        	status = expcartruck();
            vehplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(4,h,v)))
		{
        	clearplates();
        	status = expcartruck();
            vehplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(5,h,v)))
		{
        	clearplates();
            status = expeqoth();
            vehplates();
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* livestock module: accesses livestock expense modules */
int livestock(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	livplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* other expenses module:  accesses other expense modules */
int expother(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	othplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(4,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* receipts module: accesses revenue modules */
int receipts(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	recplates();
	extrabutts();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(4,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(5,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(6,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(7,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	remextrabut();
	return(status);
}

/* summaries module: accesses farm summary modules */
int summaries(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	sumplates();
	extrabutts();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
			clearplates();
			status = earnings();
			sumplates();
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	remextrabut();
	return(status);
}

/* earnings module: accesses retained earnings modules */
int earnings(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	earplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(4,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(5,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* physical module: accesses physical farm information ie. land */
int physical(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	phyplates();
	extrabutts();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
			clearplates();
			status = livestock1();
			phyplates();			
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
			clearplates();
			status = land();
			phyplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
			clearplates();
			status = labour();
			phyplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	remextrabut();
	return(status);
}

/* livestock1 module: accesses livestock physical records modules */
int livestock1(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	liv1plates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* land module: accesses land physical records modules */
int land(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	lanplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* labour module: accesses hired labour information modules */
int labour(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	labplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* enterprise module: accesses farm enterprise information modules */
int enterprise(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	entplates();
	extrabutts();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(4,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	remextrabut();
	return(status);
}

/* realestate module: accesses realestate inventory modules */
int realestate(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	realplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
			clearplates();
			status = ownedreal();
			realplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
			clearplates();
			status = rentreal();
			realplates();
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* ownedreal module: accesses owned realestate inventory modules */
int ownedreal(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	rentplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
			clearplates();
			status = ownbare();
			rentplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
			clearplates();
			status = ownimp();
			rentplates();
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* rentreal module: accesses rented realestate inventory modules */
int rentreal(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	rentplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
			clearplates();
			status = rentbare();
			rentplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
			clearplates();
			status = rentimp();
			rentplates();
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* ownbare module: owned, bare land information module */
int ownbare(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(0,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(0,LABOVE);
	clearplates();
	return(status);
}

/* ownimp module: owned, improved land information module */
int ownimp(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(1,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(1,LABOVE);
	clearplates();
	return(status);
}

/* rentbare module: rented, bare land information module */
int rentbare(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(0,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(0,LABOVE);
	clearplates();
	return(status);
}

/* rentimp module: rented, improved land information module */
int rentimp(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(2,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(2,LABOVE);
	clearplates();
	return(status);
}

/* invfarmpers: access farm and personal, A/R and A/P account modules */
int invfarmpers(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	invfarmarplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
			clearplates();
			status = invfarmar();
			invfarmarplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
			clearplates();
			status = invpersar();
			invfarmarplates();
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* invfarmar module: accesses farm A/R and A/P modules */
int invfarmar(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	invpersapplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
			clearplates();
			status = invfarmarLIS();
			invpersapplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
			clearplates();
			status = invfar();
			invpersapplates();
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* invfar module: farm A/R information module */
int invfar(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(3,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(3,LABOVE);
	clearplates();
	return(status);
}

/* invfarmarLIS module: accesses long, intermediate, and short A/P modules */
int invfarmarLIS(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	LISplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
			clearplates();
			status = invfap();
			LISplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
			clearplates();
			status = invfap();
			LISplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
			clearplates();
			status = invfap();
			LISplates();
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* invfap module: farm A/P information module */
int invfap(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(4,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(4,LABOVE);
	clearplates();
	return(status);
}

/* invpersar module: accesses personal A/R and A/P modules */
int invpersar(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	invpersapplates();

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
			clearplates();
			status = invpap();
			invpersapplates();
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
			clearplates();
			status = invpar();
			invpersapplates();
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	clearplates();
	return(status);
}

/* invpar module: personal A/R information module */
int invpar(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(3,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(3,LABOVE);
	clearplates();
	return(status);
}

/* invpap module: personal A/P information module */
int invpap(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(4,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(4,LABOVE);
	clearplates();
	return(status);
}

/* invhouse module: household and personal inventory information module */
int invhouse(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(5,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(5,LABOVE);
	clearplates();
	return(status);
}

/* invmachinery module: accesses machinery inventory modules */
int invmachinery(void)
{
	int key,
		button = 1,
		status = 0,
		h, v;

	machineplates();
	do{
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
/*			clearplates();
			status = farmpower();
			machineplates(); */
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
/*			clearplates();
			status = cropping();
			machineplates(); */
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
/*			clearplates();
			status = haying();
			machineplates(); */
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(3,h,v)))
		{
/*			clearplates();
			status = specialcrops();
			machineplates(); */
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(4,h,v)))
		{
/*			clearplates();
			status = dairy();
			machineplates(); */
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(5,h,v)))
		{
/*			clearplates();
			status = swine();
			machineplates(); */
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(6,h,v)))
		{
/*			clearplates();
			status = poultry();
			machineplates(); */
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(7,h,v)))
		{
/*			clearplates();
			status = genlvstock();
			machineplates(); */
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(8,h,v)))
		{
/*			clearplates();
			status = misc();
			machineplates(); */
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(9,h,v)))
		{
/*			clearplates();
			status = landlords();
			machineplates(); */
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
			if(button)
			{
				buttonoff(1);
				button = 0;
			}
			else
			{
				buttonon(1);
				button = 1;
			}
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
			if(button)
			{
				buttonoff(1);
				button = 0;
			}
			else
			{
				buttonon(1);
				button = 1;
			}
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if (status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));
	clearplates();
	return(status);
}


/* expspecial module: special bulk purchases information module */
int expspecial(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(6,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(6,LABOVE);
	clearplates();
	return(status);
}


/* invgrainfeed module: stores inventory of grain and feed levels and */
/* breaks down quantities							  */
int invgrainfeed(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
	showform(7,LLEFT);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

	eraseform(7,LLEFT);
	clearplates();
	return(status);
}

/* expcartruck module: car and truck expense information module */
int expcartruck(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(8,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(8,LABOVE);
	clearplates();
	return(status);
}


/* expeqoth module: other equipment expense information module */
int expeqoth(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(9,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(9,LABOVE);
	clearplates();
	return(status);
}

/* expbldg module: building and repairs expense information module */
int expbldg(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(10,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(10,LABOVE);
	clearplates();
	return(status);
}

/* expcrop module: cropping expense information module */
int expcrop(void)
{
	int key,
		buton = 1,
		status = 0,
		h,
		v;

	formplates();
/*	sprdsht(0,5); */
	showform(11,LABOVE);

	/* while exit not requested */
	do {
		key = checkinput();
		h = rat.horiz;
		v = rat.verti;

		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(0,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(1,h,v)))
		{
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (platehit(2,h,v)))
		{
		}
		if((rat.buttons == RBUTTON && key == HELD) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == CBUTTON && key == RELEASE) && (checkhit(1,h,v)))
		{
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
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(2,h,v)))
		{
			status = 1;
		}
		if((rat.buttons == LBUTTON && key == PRESS) && (checkhit(0,h,v)))
		{
			status = 2;
		}
		if(status == 2 || status == 1) key = ESC;
	} while(!(((rat.buttons == LBUTTON && key == RELEASE) || (key == ENTER))
			&& checkhit(3,h,v) || key == ESC));

/*	removesprdsht(0,10); */
	eraseform(11,LABOVE);
	clearplates();
	return(status);
}

/* defineforms module: defines forms for use in application modules */
void defineforms(void)
{
/*	defform("0,100,15,Description,212,100,10,Value,354,100,10,Area"); */
	defform("0,100,15,Description,"
			"0,200,10,Tax Value,143,200,8,Improved,"
			"258,200,8,Unimproved,373,200,8,Total Area,"
			"0,300,8,Changes,115,300,8,Adjustments,"
			"335,300,10,Beginning,480,300,10,End of Year");
	defform("0,100,20,Improvements,"
			"0,200,10,New Impr.,143,200,8,Sold,"
			"258,200,8,Adjustments,"
			"0,300,8,Rate,115,300,8,Amount,"
			"335,300,10,Beginning,"
			"480,300,10,End of Year");
	defform("0,100,20,Improvements,"
			"0,200,10,Capital Exp.,143,200,8,Sold,"
			"258,200,8,Adjustments,"
			"0,300,8,Rate,115,300,8,Amount,"
			"335,300,10,Beginning,"
			"480,300,10,End of Year");
	defform("0,100,20,Type of Acct,"
			"0,200,10,By whom,143,200,8,New Accts,"
			"258,200,8,Principal,"
			"373,200,8,Interest,"
			"335,300,10,Beginning,"
			"480,300,10,End of Year");
	defform("0,100,20,Type of Loan,"
			"0,200,10,To whom,143,200,8,New Debts,"
			"258,200,8,Principal,"
			"373,200,8,Interest,"
			"335,300,10,Beginning,"
			"480,300,10,End of Year");
	defform("0,100,20,Inventory Type,"
			"335,100,10,Beginning,"
			"480,100,10,End of Year");
    defform("0,75,20,Description,0,125,8,Date,"
    		"135,125,10,Quantity,300,125,10,Cash Paid,"
            "300,175,10,Deisel,300,225,10,Gas,"
            "300,275,10,Oil,300,325,10,Grease,"
            "300,375,10,Other");
	defform("160,50,3,Beg Grade,490,50,3,End Grade,"
			"160,80,7,Units,490,80,7,Units,"
			"160,110,10,Operator $,490,110,10,Operator $,"
			"160,140,10,Landlord $,490,140,10,Landlord $,"
			"160,230,7,Qty Produced,490,200,7,Qty Used Seed,"
			"160,260,7,Qty Purchased,490,230,7,Qty Sold,"
			"160,320,8,Total Qty,490,260,7,Landlord Share,"
			"490,290,7,Qty Used Feed,490,320,8,Total Qty");
    defform("0,75,8,Date,0,150,20,Description,"
    		"0,225,10,Cash,143,225,10,Gas etc.,"
            "286,225,10,Repairs etc.,429,225,10,Other");
    defform("0,75,8,Date,0,150,20,Description,"
    		"0,225,10,Cash,143,225,10,Gen Copping,"
            "286,225,10,Haying,429,225,10,Special Crop,"
			"0,300,10,Cattle,143,300,10,Swine,"
			"286,300,10,Poultry,429,300,10,Other");
    defform("0,75,8,Date,0,150,20,Description,"
    		"286,150,10,Cash,0,225,10,House,"
            "143,225,10,Sheds/Bins,286,225,10,Barn/Shelter,"
			"429,225,10,Fences/wells");
    defform("0,75,8,Date,0,150,20,Description,"
			"286,150,10,Cash,0,225,10,Chemical etc,"
			"143,225,10,Custom Work,286,225,10,Other,"
			"0,300,10,Seed,143,300,10,Supplies etc");
}

/* definetables module: defines tables for storing information */
int definetables(void)
{
    char *ffld1[] = {"Descrip","Tax Value","Improved","Unimproved",
    					"Total Area","Beg Value","Changes",
                        "Adjustments","End Value"};
    char *ftyp1[] = {"A20","$","N","N","N","$","$","$","$"};
    char *ffld2[] = {"Descrip","Beg Value","New","Sold","Adjustments",
    					"Dep Rate","Dep Amount","End Value"};
    char *ftyp2[] = {"A20","$","$","$","$","N","$","$"};
	char *ffld3[] = {"Grain","Beg Grade","Beg Units","Beg U Price",
						"Beg Op","Beg Landl","Qty Prod","Qty Purch",
						"Tot Expect","Qty Seed","Qty Sold Op",
						"Qty Sold Ld","Qty Livest","End Grade","End Units",
						"End U Price","End Op","End Landl","Tot Act"};
    char *ftyp3[] = {"A20","A3","N","N","N","N","N","N","N","N","N",
						"N","N","A3","N","N","N","N","N"};
    char *ffld4[] = {"Type","When","Descrip","Whom","Beg Value","New",
    					"Princ","Interest","End Value"};
    char *ftyp4[] = {"N","N","A20","A20","$","$","$","$","$"};
    char *ffld5[] = {"Description","Beg Value","End Value"};
    char *ftyp5[] = {"A20","$","$"};
    char *ffld6[] = {"Date","Descrip","Quantity","Cash paid","Diesel",
    					"Gas","Oil","Grease","Other"};
    char *ftyp6[] = {"D","A20","N","$","$","$","$","$","$"};
    char *ffld7[] = {"Date","Descrip","Cash Paid","Gas etc","Repairs etc",
    					"Other"};
    char *ftyp7[] = {"D","A20","$","$","$","$"};

    setuptable("irealb",ffld1,ftyp1);
    setuptable("ireali",ffld2,ftyp2);
	setuptable("ignf",ffld3,ftyp3);
    setuptable("ifarmar",ffld4,ftyp4);
    setuptable("ihouse",ffld5,ftyp5);
    setuptable("spclacct",ffld6,ftyp6);
    setuptable("expcar",ffld7,ftyp7);
    return(0);
}

/* checkinput module: checks for user input via input device hardware */
int checkinput(void)
{
	int buts;

	buts = getratinput(&rat);
	if(keypress()) return(Get_key());
	return(buts);
}

/* mainbuttons module: buttons for use in main() */
void mainbuttons(void)
{
	/* Button */
	defbutton(100,450,"End");
	defbutton(400,450,"Help!");
}

/* mainplates module: plates for use in main() */
void mainplates(void)
{
	/* Plate */	
	defplate(0,25,"Inventory");
	defplate(58,25,"Expenses");
	defplate(110,25,"Receipts");
	defplate(162,25,"Household");
	defplate(220,25,"Farm Summary");
	defplate(296,25,"Physical Records");
	defplate(396,25,"Enterprise Summary");
	defplate(508,25,"Income Tax");
}

/* extrabutts module: extra buttons for use deeper into application */
void extrabutts(void)
{
	/* Button */
	defbutton(170,450,"Main");
	defbutton(260,450,"Previous");
}

/* remextrabut module: removes extra buttons from use */
void remextrabut(void)
{
	/* Button */
	remove_button(2);
	remove_button(3);
}

/* invplates module: plates for use in inventory() */
void invplates(void)
{
	defplate(0,25,"Real Estate");
	defplate(70,25,"Machinary & Equipment");
	defplate(200,25,"Livestock");
	defplate(258,25,"Grain, Feed, Supplies");
	defplate(388,25,"Farm & Pers  A/R & A/P");
	defplate(524,25,"Personal");
	defplate(576,25,"Net Worth");
}

/* vehplates module: plates for use in vehicles() */
void vehplates(void)
{
	defplate(0,25,"Car");
	defplate(22,25,"Truck");
	defplate(56,25,"Tractor");
	defplate(102,25,"Combine");
    defplate(148,25,"Swather");
	defplate(194,25,"Other");
}

/* livplates module: plates for use in livestock() */
void livplates(void)
{
	defplate(0,25,"Cattle");
	defplate(40,25,"Swine");
	defplate(74,25,"Poultry");
	defplate(120,25,"Other");
}

/* othplates module: plates for use in other()? */
void othplates(void)
{
	defplate(0,25,"Tools");
	defplate(34,25,"Capital");
	defplate(80,25,"Charge Accts");
	defplate(156,25,"Loans");
	defplate(190,25,"Rent");
}

/* expplates module: plates for use in expenses() */
void expplates(void)
{
	defplate(0,25,"Bulk Purchases");
	defplate(88,25,"Vehicles");
	defplate(140,25,"Buildings & Improvements");
	defplate(288,25,"Crops");
	defplate(322,25,"Livestock");
	defplate(380,25,"Other");
	defplate(414,25,"Hired Labour");
	defplate(490,25,"Farm Overhead");
}

/* recplates module: plates for use in receipts() */
void recplates(void)
{
	defplate(0,25,"Crop");
	defplate(28,25,"Misc. Crop");
	defplate(92,25,"Livestock");
	defplate(150,25,"Milk/Cream");
	defplate(214,25,"Eggs");
	defplate(242,25,"Captial Sales");
	defplate(324,25,"Borrowed Money");
	defplate(412,25,"Old Accounts Received");
}

/* sumplates module: plates for use in summary() */
void sumplates(void)
{
	defplate(0,25,"Cash Flow");
	defplate(58,25,"Cash Balance");
	defplate(134,25,"Inventories");
	defplate(204,25,"Earnings");
}

/* earplates module: plates for use in earnings() */
void earplates(void)
{
	defplate(0,25,"Gross Profit");
	defplate(76,25,"Operating Expense");
	defplate(182,25,"Earnings Summary");
	defplate(282,25,"Labour");
	defplate(322,25,"Performance");
	defplate(392,25,"Return on Captial");
}

/* phyplates module: plates for use in physical() */
void phyplates(void)
{
	defplate(0,25,"Livestock");
	defplate(58,25,"Land");
	defplate(86,25,"Labour");
	defplate(126,25,"Feed Use");
}

/* liv1plates module: plates for use in physical-livestock() */
void liv1plates(void)
{
	defplate(0,25,"Monthly Costs");
	defplate(82,25,"Production");
}

/* lanplates module: plates for use in physical-land() */
void lanplates(void)
{
	defplate(0,25,"Maps");
	defplate(28,25,"Description");
	defplate(98,25,"Useage");
	defplate(138,25,"Production");
}

/* labplates module: plates for use in labour() */
void labplates(void)
{
	defplate(0,25,"Useage");
	defplate(40,25,"Supply");
}

/* entplates module: plates for use in enterprise() */
void entplates(void)
{
	defplate(0,25,"Fixed Costs");
	defplate(70,25,"Joint Costs");
	defplate(140,25,"Crop Costs");
	defplate(204,25,"Crop Profits");
	defplate(280,25,"Livestock Profits");
}

/* realplates module: plates for use in inventory-realestate() */
void realplates(void)
{
	defplate(0,25,"Owned");
	defplate(34,25,"Rented");
}

/* rentplates module: plates for use in rented-realestate() */
void rentplates(void)
{
	defplate(0,25,"Bare Land");
	defplate(58,25,"Improvements");
}

/* invfarmarplates module: plates for use in inventory-A/R & A/P() */
void invfarmarplates(void)
{
	defplate(0,25,"Farm");
	defplate(28,25,"Personal");
}

/* invpersapplates module: plates for use in personal-A/R & A/P() */
void invpersapplates(void)
{
	defplate(0,25,"A/P");
	defplate(22,25,"A/R");
}

/* LISplates module: plates for use in farm-A/P() */
void LISplates(void)
{
	defplate(0,25,"Long Term");
	defplate(58,25,"Intermediate");
	defplate(134,25,"Short Term");
}

/* machineplates module: plates for use in invmachinery() */
void machineplates(void)
{
	defplate(0,25,"Farm Power");
	defplate(64,25,"General Crops");
	defplate(146,25,"Haying");
	defplate(186,25,"Special Crops");
	defplate(268,25,"Cattle");
	defplate(308,25,"Swine");
	defplate(342,25,"Poultry");
	defplate(388,25,"General Livestock");
	defplate(494,25,"Miscellaneous");
	defplate(576,25,"LandLords");
}

/* formplates module: plates for use when form displayed */
void formplates(void)
{
	defplate(0,25,"Previous");
	defplate(52,25,"Next");
	defplate(80,25,"Done");
}

/* help module: meant to display help information when button pressed */
void help(int value)
{
	value++;
}