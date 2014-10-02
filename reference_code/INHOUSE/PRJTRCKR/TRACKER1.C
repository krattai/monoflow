#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <graphics.h>

/* Functions Declarartion */
int *projnum;
char *proname;
main()
{
	clrscr();
	window(1, 1, 80, 25);
	printf("\n                              PROJECT TRACKER PROGRAM");
	printf("\n\n\n\n\n          Project Number:  \n");
	printf("          Project Name:  \n\n");
	printf("	  Proposer:  \n");
	printf("	  Project Status:  \n\n");
	printf("	  Completion Date\n");
	printf("	  ---------------\n");
	printf("	  Estimated:             Actual:  \n\n");
	printf("	  Project Leader         Project Team\n");
	printf("	  --------------         ------------");
	gotoxy(27, 7);
	scanf("%d", projnum);
	gotoxy(27, 8);
	scanf("%s", proname);
}