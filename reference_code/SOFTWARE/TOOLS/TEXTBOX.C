/* TEXTBOX.C: Routine to draw boxes in text mode

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/
/* The style argument determines the type of box drawn: */
/*		0 = no box 										*/
/*		1 = single-scored 								*/
/*		2 = double-scored 								*/

#include <conio.h>
#include "textbox.h"

void textbox(int left, int top, int right, int bottom, int style)
{
	register r, c;
	static bord[][6] = {				/* border characters */
			{196, 179, 218, 191, 217, 192},
			{205, 186, 201, 187, 188, 200}
	};

	if(style == 0) return;				/* no action */
	--style;							/* index to border set */

	/* Draw horizontals */
	for(c = left + 1; c < right; c++)
	{
		gotoxy(c, top);					/* top */
		cprintf("%c", bord[style][0]);
		gotoxy(c, bottom);				/* bottom */
		cprintf("%c", bord[style][0]);
	}

	/* Draw verticles */
	for(r = top + 1; r < bottom; r++)
	{
		gotoxy(left, r);				/* left */
		cprintf("%c", bord[style][1]);
		gotoxy(right, r);				/* right */
		cprintf("%c", bord[style][1]);
	}

	/* Set corners */
	gotoxy(left, top);
	cprintf("%c", bord[style][2]);
	gotoxy(right, top);
	cprintf("%c", bord[style][3]);
	gotoxy(right, bottom);
	cprintf("%c", bord[style][4]);
	gotoxy(left, bottom);
	cprintf("%c", bord[style][5]);
}