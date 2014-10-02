/* PUTSORT.C: Test sorting and searching techniques

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#define MAX			10000

void create_plates(void);
void bubble_sort(void);

int plates[MAX];
int copy[MAX];

void main()
{
	int x;
	int y;
	int bad;
	int search_plate;

	clrscr();
	printf("Creating array.\nPlease wait...");
	create_plates();
	search_plate = plates[MAX-1];
	printf("\nPress any key to continue...");
	getch();
	clrscr();
	printf("Copying array for bubble sort.\nPlease wait...");
	for(x=0;x<MAX;x++) copy[x] = plates[x];
	printf("\nArray copied.  Press any key to continue...");
	getch();
	bubble_sort();
	getch();
}

void create_plates(void)
{
	int x, y, bad;
	FILE *fp;

	if(!(fp=fopen("rndarray.dat","rb")))
	{
		printf("\nCannot open file.");
	}
	else
	{
		fread(&plates[0],sizeof(int),MAX-1,fp);
		fclose(fp);
	}
}

void bubble_sort(void)
{
	int x,y,temp;
	FILE *fp;

	if(!(fp=fopen("srtarray.dat","wb")))
	{
		printf("\nCannot open file.");
	}
	else
	{
		putchar(7);
		for(x=MAX-1;x>=0;x--)
		{
			for(y=0;y<x;y++)
			{
				if(copy[y] > copy[y+1])
				{
					temp = copy[y];
					copy[y] = copy[y+1];
					copy[y+1] = temp;
				}
			}
		}
		putchar(7);
		printf("\nArray sorted.");
		fwrite(&copy[0],sizeof(int),MAX-1,fp);
		fclose(fp);
	}
	getch();
}
