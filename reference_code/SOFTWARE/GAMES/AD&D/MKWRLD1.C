/* MKWRLD1.C: World designer 1

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<graphics.h>

/* contants */
#define		MAXX			80
#define		MAXY			300

/* structure definitions */
typedef struct{
	unsigned bit0: 1;
	unsigned bit1: 1;
	unsigned bit2: 1;
	unsigned bit3: 1;
	unsigned bit4: 1;
	unsigned bit5: 1;
	unsigned bit6: 1;
	unsigned bit7: 1;
} byte;

/* function prototypes */
int rnd(int);
int dice(int,int);
void make_land(void /* byte[][],byte[][] */);
void disp_land(void /* byte[][],byte[][] */);

/* global variables */

main()
{
	int x,
		y,
		numb,
		ht;
/*	byte land_ht1[MAXX][MAXY],
		 land_ht2[MAXX][MAXY]; */


	clrscr();
	randomize ();

	clrscr();

	printf("Please wait...\n");
	make_land(/*land_ht1,land_ht2*/);
	clrscr();
	disp_land(/*land_ht1,land_ht2*/);
	getch();
	closegraph();
}

int dice(int die, int times)
{
	int x, z, newsum = 0;

	if(die!=100)
	{
		for(x=0;x<times;x++)
		{
			do
			{
				z = rnd(die);
			} while(z==0);
			newsum += z;
		}
		return(newsum);
	}
	else
		return(rnd(die));
}

int rnd(int number)
{
	/* selects a random number between 1 and 20 */
	number++;
/*	srand((unsigned) random(5000));*/
	return(random(number));
}

void make_land(void /* byte land1[][MAXY],byte land2[][MAXY] */)
{
	int x,y,temp;
	FILE *fp1,
		fp2;
	char name1[] = "land1.dat",
		name2[] = "land2.dat";

	if(!(fp1=fopen(name1,"wb")) || !(fp2=fopen(name2,"wb")))
	{
		cprintf("\nCannot open file.");
	}
	else
	{
		for(x=0;x<MAXX;x++)
		{
			for(y=0;y<MAXY;y++)
			{
				temp = dice(2,1);
				if(temp == 1)
				{
					land1/*[x][y]*/.bit0 = 1;
					temp = dice(2,1);
					if(temp == 1)
						land2/*[x][y]*/.bit0 = 0;
					else
						land2/*[x][y]*/.bit0 = 1;
				}
				else
				{
					land1/*[x][y]*/.bit0 = 0;
				}
				temp = dice(2,1);
				if(temp == 1)
				{
					land1/*[x][y]*/.bit1 = 1;
					temp = dice(2,1);
					if(temp == 1)
						land2/*[x][y]*/.bit1 = 0;
					else
						land2/*[x][y]*/.bit1 = 1;
				}
				else
				{
					land1/*[x][y]*/.bit1 = 0;
				}
				temp = dice(2,1);
				if(temp == 1)
				{
					land1/*[x][y]*/.bit2 = 1;
					temp = dice(2,1);
					if(temp == 1)
						land2/*[x][y]*/.bit2 = 0;
					else
						land2/*[x][y]*/.bit2 = 1;
				}
				else
				{
					land1/*[x][y]*/.bit2 = 0;
				}
				temp = dice(2,1);
				if(temp == 1)
				{
					land1/*[x][y]*/.bit3 = 1;
					temp = dice(2,1);
					if(temp == 1)
						land2/*[x][y]*/.bit3 = 0;
					else
						land2/*[x][y]*/.bit3 = 1;
				}
				else
				{
					land1/*[x][y]*/.bit3 = 0;
				}
				temp = dice(2,1);
				if(temp == 1)
				{
					land1/*[x][y]*/.bit4 = 1;
					temp = dice(2,1);
					if(temp == 1)
						land2/*[x][y]*/.bit4 = 0;
					else
						land2/*[x][y]*/.bit4 = 1;
				}
				else
				{
					land1/*[x][y]*/.bit4 = 0;
				}
				temp = dice(2,1);
				if(temp == 1)
				{
					land1/*[x][y]*/.bit5 = 1;
					temp = dice(2,1);
					if(temp == 1)
						land2/*[x][y]*/.bit5 = 0;
					else
						land2/*[x][y]*/.bit5 = 1;
				}
				else
				{
					land1/*[x][y]*/.bit5 = 0;
				}
				temp = dice(2,1);
				if(temp == 1)
				{
					land1/*[x][y]*/.bit6 = 1;
					temp = dice(2,1);
					if(temp == 1)
						land2/*[x][y]*/.bit6 = 0;
					else
						land2/*[x][y]*/.bit6 = 1;
				}
				else
				{
					land1/*[x][y]*/.bit6 = 0;
				}
				temp = dice(2,1);
				if(temp == 1)
				{
					land1/*[x][y]*/.bit7 = 1;
					temp = dice(2,1);
					if(temp == 1)
						land2/*[x][y]*/.bit7 = 0;
					else
						land2/*[x][y]*/.bit7 = 1;
				}
				else
				{
					land1/*[x][y]*/.bit7 = 0;
				}
				fwrite(&land1,sizeof(byte),1,fp1);
				fwrite(&land2,sizeof(byte),1,fp2);
			}
		}
		fclose(fp1);
		fclose(fp2);
	}
}

void disp_land(void /* byte land1[][MAXY],byte land2[][MAXY] */)
{
	int x,
		y,
		color = 0;
	int graphdriver = VGA, graphmode = VGAHI;
	byte land[MAXX][MAXY];
	FILE *fp;
	char name[] = "land.dat";

	if(!(fp=fopen(name,"rb")))
	{
		cprintf("\nCannot open file.");
	}
	else
	{
		initgraph(&graphdriver, &graphmode, "");
		for(y=0;y<MAXY;y++)
		{
			for(x=0;x<MAXX;x++)
			{
				fread(&name,sizeof(char),1,fp);
				if(land1[x][y].bit0 == 0)
				{
					putpixel(x*8,y,color);
				}
				else
				{
					if(land2[x][y].bit0 == 0)
					{
						if(color > 0) color--;
						putpixel(x*8,y,color);
					}
					else
					{
						if(color < 15) color++;
						putpixel(x*8,y,color);
					}
				}
				if(land1[x][y].bit1 == 0)
				{
					putpixel(x*8+1,y,color);
				}
				else
				{
					if(land2[x][y].bit1 == 0)
					{
						if(color > 0) color--;
						putpixel(x*8+1,y,color);
					}
					else
					{
						if(color < 15) color++;
						putpixel(x*8+1,y,color);
					}
				}
				if(land1[x][y].bit2 == 0)
				{
					putpixel(x*8+2,y,color);
				}
				else
				{
					if(land2[x][y].bit2 == 0)
					{
						if(color > 0) color--;
						putpixel(x*8+2,y,color);
					}
					else
					{
						if(color < 15) color--;
						putpixel(x*8+2,y,color);
					}
				}
				if(land1[x][y].bit3 == 0)
				{
					putpixel(x*8+3,y,color);
				}
				else
				{
					if(land2[x][y].bit3 == 0)
					{
						if(color > 0) color--;
						putpixel(x*8+3,y,color);
					}
					else
					{
						if(color < 15) color--;
						putpixel(x*8+3,y,color);
					}
				}
				if(land1[x][y].bit4 == 0)
				{
					putpixel(x*8+4,y,color);
				}
				else
				{
					if(land2[x][y].bit4 == 0)
					{
						if(color > 0) color--;
						putpixel(x*8+4,y,color);
					}
					else
					{
						if(color < 15) color--;
						putpixel(x*8+4,y,color);
					}
				}
				if(land1[x][y].bit5 == 0)
				{
					putpixel(x*8+5,y,color);
				}
				else
				{
					if(land2[x][y].bit5 == 0)
					{
						if(color > 0) color--;
						putpixel(x*8+5,y,color);
					}
					else
					{
						if(color < 15) color--;
						putpixel(x*8+5,y,color);
					}
				}
				if(land1[x][y].bit6 == 0)
				{
					putpixel(x*8+6,y,color);
				}
				else
				{
					if(land2[x][y].bit6 == 0)
					{
						if(color > 0) color--;
						putpixel(x*8+6,y,color);
					}
					else
					{
						if(color < 15) color--;
						putpixel(x*8+6,y,color);
					}
				}
				if(land1[x][y].bit7 == 0)
				{
					putpixel(x*8+7,y,color);
				}
				else
				{
					if(land2[x][y].bit7 == 0)
					{
						if(color > 0) color--;
						putpixel(x*8+7,y,color);
					}
					else
					{
						if(color < 15) color--;
						putpixel(x*8+7,y,color);
					}
				}
			}
		}
	}
}
/*
character read_char(char name[])
{
	FILE *fp;
	character n;
	int x;

	n.name[0] = NULL;
	for(x=0;x<=12;x++)
	{
		if((name[x] < 'A' || name[x] > 'Z') &&
		   (name[x] < 'a' || name[x] > 'z') &&
		   (name[x] != '.'))
			name[x] = NULL;
	}

	if(!(fp=fopen(name,"rb")))
	{
		putchar(7);
		cprintf("\nCharacters does not exist.");
	}
	else
	{
		fclose(fp);
	}
	return(n);
}
	*/