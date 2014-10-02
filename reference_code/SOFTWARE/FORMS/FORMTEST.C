/* FORMTEST.C: Test form procedures and funtions

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/

#include <stdio.h>
#include <conio.h>
#include "form.h"
#include "keyio.h"

void main(void)
{
	/* Define forms in the following format: x-row, y-column, label, etc. */
	char form1[] = 	"1,1,This is top/left corner!:,"
					"1,10,The middle:,"
					"1,22,Almost the bottom:";
	char form2[] =	"1,1,This is the end!";
	char form3[] =	"1,1,Project Number:,"
					"1,2,Project Name:,"
					"1,4,Proposer:,"
					"1,5,Project Status:,"
					"1,7,Estimated Completion:,"
					"40,7,Actual Completion:";

	/* information can either be empty... */
	char info1[200] = "";

	/* or it can contain "pre-parsed" info */
	char info2[] = ",Information field 1,"
					"Information field 2,"
					"Information field 3";

	char info3[300] = "";

	/* you can even set it up this way, and then... */
	char info4[] = "Top Mid Bot";

	struct text_info original;

	/* ready info for display, like this... */
	Parse_coded_info(info2);

	/* or like this */
	info4[3]=SPLIT;
	info4[7]=SPLIT;

	/* you should always get original screen information prior to changes */
	gettextinfo(&original);

	/* this will set up a "window" with YELLOW characters on BLUE back */
	Put_window(1,1,80,25,YELLOW,BLUE);

	/* Show a form (must be done prior to putting or getting) */
	Show_form(form1);
	gotoxy(1,24);
	cprintf("Press F2 when you are finished!");

	/* then you can get a form... */
	Get_form_info(form1,info1);

	Show_form(form1);

	/* or you can load a form with info */
	Put_form_info(form1,info1);

	/* if displaying info, you should hold the screen until key pressed */
	gotoxy(1,24);
	cprintf("Press any key to continue...");
	getch();

	/* more examples of get and put info */
	Show_form(form3);
	gotoxy(1,23);
	cprintf("Press F2 when you are finished!");
	Get_form_info(form3,info3);

	Show_form(form1);
	Put_form_info(form1,info4);
	gotoxy(1,24);
	cprintf("Press any key to continue...");
	getch();

	Show_form(form1);
	Put_form_info(form1,info2);
	gotoxy(1,24);
	cprintf("Press any key to continue...");
	getch();

	Show_form(form3);
	Put_form_info(form3,info3);
	gotoxy(1,23);
	cprintf("Press any key to continue...");
	getch();

	Show_form(form3);

	/* if info already exits, you may "edit" it using Get_form_info() */
	gotoxy(1,23);
	cprintf("Press F2 when you are finished!");
	Get_form_info(form3,info3);

	Show_form(form3);
	Put_form_info(form3,info3);
	gotoxy(1,23);
	cprintf("Press any key to continue...");
	getch();

	/* You can also use forms to place remarks, like so */
	Show_form(form2);
	gotoxy(1,25);
	cprintf("Press any key to continue... By the way, 25 is bottom!");
	getch();

	/* out of courtesy, always return the screen to original attributes */
	Put_original(original);
	clrscr();

}