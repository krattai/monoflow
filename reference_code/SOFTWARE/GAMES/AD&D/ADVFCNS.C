/* ADVFCNS.C:  Adventure game funtions

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

	Functions to include:
		Fight determiner
		Giving experience points
		To determine wandering monster and treasure with it
		To determine/interpret user commands
		Creating visual environment such as windows
		Process control (after user input)
		Storing/retrieving disk information
*/

/* Header files */
#include <time.h>
#include <popup.h>
#include <stdio.h>
#include <cursor.h>
#include <stdlib.h>
#include <textbox.h>
#include <biosarea.h>
#include "adv.h"

int damage(int die, int magic, int strength)
{ 											     /* function to determine damage */
	int dam;			/* damage occuring */

	dam = dice(die,1);
	dam = dam + magic + strength;
	if(dam < 1) dam = 1;
	return (dam);
}
/* --------------------------------------------------------- */

int dice(int die, int times)      /* function to determine dice rolls */
{
	int counter,      /* counter for times */
		 rnddie,       /* random die roll   */
		 newsum = 0;   /* sum of die rolls  */

	if(die != 100) {
		for(counter = 0; counter < times; counter++) {
			do {
				rnddie = rnd(die);
			}
			while(rnddie == 0);
			newsum += rnddie;
		}
		return (newsum);
	}
	else
		return (rnd(die));
}

/* --------------------------------------------------------- */

int distance(void)						/* determines distance of monster */
{
	int die;				/* die roll */

	die = dice(6,2) * 10;
	return (die);
}

/* --------------------------------------------------------- */

int doors(int strength)                   /* determines if door opens */
{
	int die,				/* die roll */
		 required;		/* required to open door */

	die = dice(6,1);
	required = 2 + strength;
	if(required < 1) required = 1;
	if(die = required) return (1);
	if(die < required) return (1);
	return (0);
}

/* --------------------------------------------------------- */

int finddoor(int class)            /* determines if secret door found */
{
	int die;				/* die roll */

	die = dice(6,1);
	if(class = 4)						/* if Elf */
		if(die < 3) return (1);
	if(die < 2) return (1);       /* else   */
	return (0);
}

/* --------------------------------------------------------- */

int findtrap(int class)							/* determines if trap found */
{
	int die;				/* die roll */

	die = dice(6,1);
	if(class = 3)                 /* if Dwarf */
		if(die < 3) return (1);
	if(die < 2) return (1);			/* else     */
	return (0);
}

/* --------------------------------------------------------- */

int hit(int tohit, int ac, int dex, int rng, int mag, int cover)
{                                        /* function to determint hit */
	int die,          /* random die roll     */
		 rth;          /* die required to hit */

	rth = tohit - ac;
	if(mag != 0) rth = rth + mag;
	if(dex != 0) rth = rth + dex + rng + cover;
	if(rth > 20) rth = 20;
	if(rth <  2) rth =  2;
	die = dice(20,1);
	if(die > rth - 1) return (1);       /* if hit, return true  (1) */
	return (0);                         /*   else, return false (0) */
}

/* --------------------------------------------------------- */

int initiative(int adj)           /* function to determine initiative */
{
	int die;				/* die roll */

	die = dice(6,1);
	die += adj;
	return (die);
}

/* --------------------------------------------------------- */

int morale(int monster, int adj)       /* determine morale of monster */
{
	int die;			/* die role */

	monster += adj;
	if(monster > 11) return (1);
	if(monster <  3) return (0);
	die = dice(6,2);
	if(die > monster) return (0);
	return (1);
}

/* --------------------------------------------------------- */

int reaction(void)			 /* function to determine monster reaction */
{
	int die,				/* die roll */
		 react = 0;		/* reaction */

	die = dice(6,2);
	switch(die) {
	case  2: react = 1; break;
	case  3:
	case  4:
	case  5: react = 2; break;
	case  6:
	case  7:
	case  8: react = 3; break;
	case  9:
	case 10:
	case 11: react = 4; break;
	default: react = 5;
	}
	return (react);
}

/* --------------------------------------------------------- */

int saving(int class, int attack, int adj)
{ 														/* determines savings throw */
	int die,       /* die roll    */
		 made = 0;  /* saving made */

	die = dice(20,1) + adj;

	switch(class) {
	case  1:
		switch(attack) {
		case  1:	if(die > 13) made = 1; break;
		case  2: if(die > 14) made = 1; break;
		case  3:	if(die > 15) made = 1; break;
		default:	if(die > 16) made = 1;
		}
		break;
	case  2:
		switch(attack) {
		case  1:	if(die > 10) made = 1; break;
		case  2: if(die > 11) made = 1; break;
		case  3:	if(die > 13) made = 1; break;
		case  4:	if(die > 15) made = 1; break;
		default: if(die > 14) made = 1;
		}
		break;
	case  3:
		switch(attack) {
		case  1: if(die >  9) made = 1; break;
		case  2:	if(die > 10) made = 1; break;
		case  3:	if(die > 11) made = 1; break;
		case  4:	if(die > 12) made = 1; break;
		default:	if(die > 13) made = 1;
		}
		break;
	case  4:
		switch(attack) {
		case  1:	if(die > 11) made = 1; break;
		case  2:
		case  3:	if(die > 12) made = 1; break;
		default:	if(die > 14) made = 1;
		}
		break;
	case  5:
		switch(attack) {
		case  1:	if(die > 11) made = 1; break;
		case  2:	if(die > 12) made = 1; break;
		case  3:	if(die > 13) made = 1; break;
		case  4:	if(die > 14) made = 1; break;
		default:	if(die > 15) made = 1;
		}
		break;
	case  6:
	case  7:
		switch(attack) {
		case  1:
		case  3:	if(die > 12) made = 1; break;
		case  2:	if(die > 13) made = 1; break;
		case  4:	if(die > 15) made = 1; break;
		default:	if(die > 14) made = 1;
		}
		break;
	default:
		if(die > 15) made = 1;
	}
	return (made);
}

/* ---------------------------------------------------------- */

int surprise(void)                  /* function to determine surprise */
{
	int die;				/* die roll */

	die = dice(6,1);
	if(die < 3) return (1);
	return (0);
}

/* ---------------------------------------------------------- */

int traps(void)				               /* determines if trap sprung */
{
	int die;				/* die roll */

	die = dice(6,1);
	if(die < 3) return (1);
	return (0);
}