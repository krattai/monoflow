/* FORMTEST.C: Test form procedures and funtions

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/

#include <stdio.h>
#include <conio.h>
#include "form.h"
#include "keyio.h"

main(void)
{
	char form1[] = 	"1,1,Hello World!:,"
					"1,10,The middle:,"
					"1,24,The bottom:";
	char form2[] =	"1,1,This is the end";
	char info1[200] = "";

	struct text_info original;

	gettextinfo(&original);

	Put_window(1,1,80,24,YELLOW,BLUE);

	clrscr();
	Show_form(form1);
	Get_form_info(form1,info1);
	clrscr();
	Show_form(form2);
	getch();

	Put_original(original);
	clrscr();

}
