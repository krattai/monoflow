/* VIDEO3.C: char output calls for Video_IO
	note that vid_wca() and vid_gca() do not advance the cursor

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/

#include<stdio.h>
#include<dos.h>
#include"video.h"

void vid_wca(int c, int a)			/* display char and attribute */
{
	union REGS sreg, dreg;

	sreg.h.bh = 0;					/* force display page 0 */
	sreg.h.bl = a;					/* attribute */
	sreg.h.al = c;					/* char */
	sreg.x.cx = 1;					/* count of chars to display = 1 */
	sreg.h.ah = V_WCA;				/* operation code */
	int86(V_INT_NO, &sreg, &dreg);
}

int vid_gca(char *pc, char *pa)		/* get char and attribute at cursor */
{
	union REGS sreg, dreg;

	sreg.h.bh = 0;					/* page zero */
	sreg.h.ah = V_GCA;				/* operation code */
	int86(V_INT_NO, &sreg, &dreg);
	*pc = dreg.h.al;				/* save char value */
	*pa = dreg.h.ah;				/* and attribute */
	return(dreg.x.ax);				/* return char and attribute */
}

void vid_tc(int c)					/* output one char with TTY write */
{
	union REGS sreg, dreg;

	sreg.h.bl = 0;					/* dummy foreground color */
	sreg.h.al = c;					/* character */
	sreg.h.ah = V_WTTY;				/* operation code */
	int86(V_INT_NO, &sreg, &dreg);
}