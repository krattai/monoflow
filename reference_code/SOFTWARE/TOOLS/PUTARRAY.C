/* PUTARRAY.C: Test sorting and searching techniques

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#define MAX			10000

void create_plates(void);

int plates[MAX];

void main()
{
	int x;
	int y;
	int bad;
	int search_plate;

	clrscr();
	printf("Creating array.\nPlease wait...");
	create_plates();
	for(x=0;x<=50;x++) putchar(7);
	search_plate = plates[MAX-1];
	printf("\nPress any key to continue...");
	getch();
}

void create_plates(void)
{
	int x, y, bad;
	FILE *fp;

	if(!(fp=fopen("rndarray.dat","wb")))
	{
		printf("\nCannot open file.");
	}
	else
	{
		for(x=0;x<MAX;x++)
		{
			bad = 1;
			while(bad)
			{
				bad = 0;
				plates[x] = random(20000);
				for(y=0;y<x;y++)
				{
					if(plates[y] == plates[x]) bad = 1;
				}
			}
			fwrite(&plates[x],sizeof(int),1,fp);
		}
		fclose(fp);
	}
}
