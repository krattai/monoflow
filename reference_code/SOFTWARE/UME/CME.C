/* CME.C: Command Module Environment Program

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/

#include<stdio.h>
#include<conio.h>
#include<dir.h>
#include<string.h>
#include"keyio.h"

void main(void)
{
	char *oldpath,*curpath;

	clrscr();
	strcpy(oldpath,"\\");
	getcurdir(0,oldpath+1);
	printf("C:%s>",oldpath);
	while(getch() != ESC)
	{
		getcurdir(0,oldpath+1);
		printf("\nC:%s>",oldpath);
	}
}