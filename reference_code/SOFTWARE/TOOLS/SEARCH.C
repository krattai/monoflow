/* SEARCH.C: Test sorting and searching techniques

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#define MAX			10000

void create_plates(void);
void bubble_sort(void);
void quick_sort(void);
void linear_search(int plate);
void binary_search(int plate);
void comb_search(int plate);


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
	clrscr();
	printf("Searching for plate %d",search_plate);
	getch();
	linear_search(search_plate);
	getch();
	binary_search(search_plate);
	getch();
/*
	create plates randomly
	store last created plate
	bubble sort plates
	quick sort plates
	search for plate by linear method
	search for plate by binary method
	do searches again on sorted plates */
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
	}
}

void bubble_sort(void)
{
	int x,y,temp;

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
	getch();
}

void quick_sort(void)
{
	int copy[MAX];
	int x;

	printf("Copying array for quick sort.\nPlease wait...");
	for(x=0;x<MAX;x++)
		copy[x] = plates[x];
	printf("\nArray copied.  Press any key to continue...");
	getch();
}

void linear_search(int plate)
{
	int x = 0;
	int found = 0;

	putchar(7);
	while(!found)
	{
		if(plate == copy[x])
		{
			putchar(7);
			printf("\nPlate %d found at position %d",plate,x);
			found = 1;
		}
		else
		{
			x++;
		}
	}
}

void binary_search(int plate)
{
	int mid,
		hi = MAX-1,
		lo = 0;
	int found = 0;

	putchar(7);
	mid = (hi+lo)/2;
	while(!found)
	{
		if(plate == copy[mid])
		{
			putchar(7);
			printf("\nPlate %d found at position %d",plate,mid);
			found = 1;
		}
		else
		{
			if(plate < copy[mid])
			{
				hi = mid;
			}
			else
			{
				lo = mid;
			}
			mid = (hi+lo)/2;
		}
	}
}

void comb_search(int plate)
{
}