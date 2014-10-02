/* POPUP.H: Prototypes and typedefs for POPUP.C library

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/

extern struct text_info;				/* from conio.h */

typedef struct {
	int		left, top, right, bottom,	/* border locations */
			style,						/* border style */
			normal, hilite,				/* text attributes */
			normback, hiback, border;
	char	*text;						/* fixed text contents */
	void	*save;						/* pointer to save buffer */
	struct	text_info prev;				/* previos video state */
} POPUP;

typedef struct {
	int		row,						/* row where bar appears */
			fore, back;					/* foreground/background colors */
	char	*choice;					/* pointer to text contents */
} MENUBAR;

void popShow(POPUP *pop);				/* display popup window */

void popErase(POPUP *pop);
				 /* Erase popup window, restoring overlaid image */
void Menu_hilite(int, int, int, char);