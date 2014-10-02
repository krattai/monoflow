/* KEYIO1.C: keyio module - single key input
	uses kbhit and getch library functions

	Created by:		Kevin Rattai
	Date created:	June 1, 1993

*/

#include<stdio.h>
#include<conio.h>
#include"keyio.h"

int keypress(void)					/* check for keyboard input waiting */
{									/* returns 0 = no input, 1 = input */
	int stat;

	return(kbhit());				/* get key available status */
}

/* getkey - waits for and returns the next keystroke input */
/* keystrokes that the ROM-BIOS describes with extended codes */
/* are returned as integers > 255 */
int getkey(void)					/* get the next key input */
{
	int c;

	c = getch();					/* get single ASCII characters */
	if(c == 0)						/* is it a non-ASCII extended code? */
		c = 0x100 + getch();		/* yes - use 256 + scan code */
	return(c);
}

int Get_key(void)
{
	char choice;

	choice = toupper(getch());
	if(choice == NULL)
		choice = toupper(getch()) * -1;
	return((int) choice);
}

int get_key(void)
{
	char choice;

	choice = getch();
	if(choice == NULL)
		choice = getch() * -1;
	return((int) choice);
}
