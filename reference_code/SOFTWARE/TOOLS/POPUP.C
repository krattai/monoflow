/* POPUP.C: Pop-up window library

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/

#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <alloc.h>
#include <string.h>
#include "textbox.h"
#include "biosarea.h"
#include "popup.h"

void popShow(POPUP *pop)				/* display popup window */
{
	int	bufsize, x;

	gettextinfo (&(pop->prev));			/* save old video state */
	window (1, 1, 80, 25);				/* go to full-screen mode */

	/* Make sure boundaries are in correct order */
	if (pop->left > pop->right)
	{
		x = pop->left;
		pop->left = pop->right;
		pop->right = x;
	}
	if (pop->top > pop->bottom)
	{
		x = pop->top;
		pop->top = pop->bottom;
		pop->bottom = x;
	}

	/* Save area under new window */
	bufsize = (pop->right - pop->left + 2) *		/* buf size */
				 (pop->bottom - pop->top + 2) * 2;
	pop->save = malloc (bufsize);				 	 /* get buffer */
	gettext (pop->left, pop->top, pop->right,	 /* save image */
				pop->bottom, pop->save);

	/* Draw the border */
	textcolor (pop->border);					  /* border color */
	textbackground (pop->normback);
	textbox (pop->left, pop->top, pop->right,		  /* border */
				pop->bottom, pop->style);

	/* Open the window */
	textcolor (pop->normal);			 /* set text foreground */
	window (pop->left+1, pop->top+1,				/* open window */
			  pop->right-1, pop->bottom-1);
	clrscr ();
	if (pop->text != NULL)					 /* write fixed text */
		cprintf("%s",pop->text);
}

void popErase(POPUP *pop) /* Erase popup window, restoring overlaid image */
{
	/* Make sure the window exists */
	if (pop->save == NULL)
		return;									/* quit if it doesn't */

	/* Restore previous window characteristics */
	window (pop->prev.winleft, pop->prev.wintop,
			pop->prev.winright, pop->prev.winbottom);
	gotoxy (pop->prev.curx, pop->prev.cury);		 /* curs pos */
	textcolor (pop->prev.attribute & 0x0F);		/* old color */
	textbackground (pop->prev.attribute >> 4);	  /* and b/g */

	/* Restore overlaid area */
	puttext (pop->left, pop->top, pop->right, pop->bottom, pop->save);
	free (pop->save);
	pop->save = NULL;
}

void popCenter(POPUP *win, char *string)	/* Center string in window */
{
	int	i, tab;

	tab = (win->right - win->left - strlen (string)) / 2;
	for (i = 0; i < tab; i++)
		cputs (" ");
	cputs (string);
}

void popRewrite(POPUP *win, int row, char attrib)
			 /* Rewrite pop-up row with new character attribute */
{
	int		p, nchars;
	union	REGS	reg;
	struct	text_info text;

	gettextinfo (&text);					 /* get current settings */
	nchars = win->right - win->left;				 /* popup width */
	for (p = 1; p < nchars; p++)
	{
		gotoxy (p, row);
		reg.h.ah = 8;								 /* get characters */
		reg.h.bh = 0;										/* in page 0 */
		int86 (0x10, &reg, &reg);					/* via ROM BIOS */
		reg.h.ah = 9;						  /* write back out with */
		reg.h.bl = attrib;						 /* hilite attribs */
		reg.h.bh = 0;											/* page 0 */
		reg.x.cx = 1;										 /* one char */
		int86 (0x10, &reg, &reg);
	}
	gotoxy (text.curx, text.cury);				/* restore curs */
}

void popHilite(POPUP *win, int row)	/* Hilight text in popup row */
{
	char attrib;

	attrib = win->hilite +					 /* hilite attributes */
				(win->hiback << 4);
	popRewrite (win, row, attrib);
}

void popNormal(POPUP *win, int row)
	/* Set text in popup row to normal attribs */
{
	char attrib;

	attrib = win->normal +				 	 /* normal attributes */
				(win->normback << 4);
	popRewrite (win, row, attrib);
}

void menubar(MENUBAR *spec) /* Write the menu bar described by spec */
{
	struct text_info	text;
	int					p, s;

	gettextinfo (&text);				 /* get current video state */
	textcolor (spec->fore);							  /* set colors */
	textbackground (spec->back);
	window(1,1,80,25);
	gotoxy (1, spec->row);					 /* start of menu bar */

	for (s = 1; s < text.winright - text.winleft + 2; s++)
		cprintf ("%c", ' ');						 /* set background */

	gotoxy (1, spec->row);					 /* start of menu bar */
	for (p = 0; spec->choice [p]; p++)	  /* copy text to bar */
		if (spec->choice [p] != ',')
			cprintf ("%c", spec->choice [p]);		  /* write char */
		else
			gotoxy (3 + wherex(), spec->row);		  /* next */

	/* Restore previous state */
	textcolor (text.attribute & 0x8F);			/* color scheme */
	textbackground (text.attribute >> 4);
	gotoxy (text.curx, text.cury);
}

void Menu_hilite(int start, int end, int row, char attrib)
{
	union REGS reg;
	int x;

	for(x = start; x <= end; x++)
	{
		gotoxy(x,row);
		reg.h.ah = 8;
		reg.h.bh = 0;
		int86(0x10, &reg, &reg);
		reg.h.ah = 9;
		reg.h.bl = attrib;
		reg.h.bh = 0;
		reg.x.cx = 1;
		int86(0x10, &reg, &reg);
	}
}
