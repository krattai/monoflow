/* MKWRLD3.C: World designer 3

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<graphics.h>

/* contants */
#define		MAXX			80
#define		MAXY			480

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
void make_land(void);
void disp_land(void);

/* global variables */

main()
{
	clrscr();
	randomize ();

	clrscr();

	printf("Making land mass.  Please wait!\n");
	make_land();
	clrscr();
	disp_land();
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
	number++;
	return(random(number));
}

void make_land(void)
{
	int x,
		y,
		temp,
		dir = 0;
	FILE *fp1,
		*fp2;
	byte land1,
		land2;
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
				if(dir == 0)
				{
					temp = dice(100,1);
				}
				else
				{
					temp = dice(5,1);
				}
				if(temp == 1)
				{
					land1.bit0 = 1;
					temp = dice(2,1);
					if(temp == 1)
					{
						dir = -1;
						land2.bit0 = 0;
					}
					else
					{
						dir = 1;
						land2.bit0 = 1;
					}
				}
				else
				{
					dir = 0;
					land1.bit0 = 0;
				}
				if(dir == 0)
				{
					temp = dice(100,1);
				}
				else
				{
					temp = dice(5,1);
				}
				if(temp == 1)
				{
					land1.bit1 = 1;
					temp = dice(2,1);
					if(temp == 1)
					{
						dir = -1;
						land2.bit1 = 0;
					}
					else
					{
						dir = 1;
						land2.bit1 = 1;
					}
				}
				else
				{
					dir = 0;
					land1.bit1 = 0;
				}
				if(dir == 0)
				{
					temp = dice(100,1);
				}
				else
				{
					temp = dice(5,1);
				}
				if(temp == 1)
				{
					land1.bit2 = 1;
					temp = dice(2,1);
					if(temp == 1)
					{
						dir = -1;
						land2.bit2 = 0;
					}
					else
					{
						dir = 1;
						land2.bit2 = 1;
					}
				}
				else
				{
					dir = 0;
					land1.bit2 = 0;
				}
				if(dir == 0)
				{
					temp = dice(100,1);
				}
				else
				{
					temp = dice(5,1);
				}
				if(temp == 1)
				{
					land1.bit3 = 1;
					temp = dice(2,1);
					if(temp == 1)
					{
						dir = -1;
						land2.bit3 = 0;
					}
					else
					{
						dir = 1;
						land2.bit3 = 1;
					}
				}
				else
				{
					dir = 0;
					land1.bit3 = 0;
				}
				if(dir == 0)
				{
					temp = dice(100,1);
				}
				else
				{
					temp = dice(5,1);
				}
				if(temp == 1)
				{
					land1.bit4 = 1;
					temp = dice(2,1);
					if(temp == 1)
					{
						dir = -1;
						land2.bit4 = 0;
					}
					else
					{
						dir = 1;
						land2.bit4 = 1;
					}
				}
				else
				{
					dir = 0;
					land1.bit4 = 0;
				}
				if(dir == 0)
				{
					temp = dice(100,1);
				}
				else
				{
					temp = dice(5,1);
				}
				if(temp == 1)
				{
					land1.bit5 = 1;
					temp = dice(2,1);
					if(temp == 1)
					{
						dir = -1;
						land2.bit5 = 0;
					}
					else
					{
						dir = 1;
						land2.bit5 = 1;
					}
				}
				else
				{
					dir = 0;
					land1.bit5 = 0;
				}
				if(dir == 0)
				{
					temp = dice(100,1);
				}
				else
				{
					temp = dice(5,1);
				}
				if(temp == 1)
				{
					land1.bit6 = 1;
					temp = dice(2,1);
					if(temp == 1)
					{
						dir = -1;
						land2.bit6 = 0;
					}
					else
					{
						dir = 1;
						land2.bit6 = 1;
					}
				}
				else
				{
					dir = 0;
					land1.bit6 = 0;
				}
				if(dir == 0)
				{
					temp = dice(100,1);
				}
				else
				{
					temp = dice(5,1);
				}
				if(temp == 1)
				{
					land1.bit7 = 1;
					temp = dice(2,1);
					if(temp == 1)
					{
						dir = -1;
						land2.bit7 = 0;
					}
					else
					{
						dir = 1;
						land2.bit7 = 1;
					}
				}
				else
				{
					dir = 0;
					land1.bit7 = 0;
				}
				fwrite(&land1,sizeof(byte),1,fp1);
				fwrite(&land2,sizeof(byte),1,fp2);
			}
			printf(".");
		}
		fclose(fp1);
		fclose(fp2);
	}
}

void disp_land(void)
{
	int x,
		y,
		color = 2,
		graphdriver = VGA,
		graphmode = VGAHI;
	byte land1,
		land2;
	FILE *fp1,
		*fp2;
	char name1[] = "land1.dat",
		name2[] = "land2.dat";

	if(!(fp1=fopen(name1,"rb")) || !(fp2=fopen(name2,"rb")))
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
				fread(&land1,sizeof(byte),1,fp1);
				fread(&land2,sizeof(byte),1,fp2);
				if(land1.bit0 == 0)
				{
					putpixel(x*8,y,color);
				}
				else
				{
					color = 0;
					if(land2.bit0 == 0)
					{
						putpixel(x*8,y,0);
					}
					else
					{
						putpixel(x*8,y,15);
					}
				}
				color = 2;
				if(land1.bit1 == 0)
				{
					putpixel(x*8+1,y,color);
				}
				else
				{
					color = 0;
					if(land2.bit1 == 0)
					{
						putpixel(x*8+1,y,0);
					}
					else
					{
						putpixel(x*8+1,y,15);
					}
				}
				color = 2;
				if(land1.bit2 == 0)
				{
					putpixel(x*8+2,y,color);
				}
				else
				{
					color = 0;
					if(land2.bit2 == 0)
					{
						putpixel(x*8+2,y,0);
					}
					else
					{
						putpixel(x*8+2,y,15);
					}
				}
				color = 2;
				if(land1.bit3 == 0)
				{
					putpixel(x*8+3,y,color);
				}
				else
				{
					color = 0;
					if(land2.bit3 == 0)
					{
						putpixel(x*8+3,y,0);
					}
					else
					{
						putpixel(x*8+3,y,15);
					}
				}
				color = 2;
				if(land1.bit4 == 0)
				{
					putpixel(x*8+4,y,color);
				}
				else
				{
					color = 0;
					if(land2.bit4 == 0)
					{
						putpixel(x*8+4,y,0);
					}
					else
					{
						putpixel(x*8+4,y,15);
					}
				}
				color = 2;
				if(land1.bit5 == 0)
				{
					putpixel(x*8+5,y,color);
				}
				else
				{
					color = 0;
					if(land2.bit5 == 0)
					{
						putpixel(x*8+5,y,0);
					}
					else
					{
						putpixel(x*8+5,y,15);
					}
				}
				color = 2;
				if(land1.bit6 == 0)
				{
					putpixel(x*8+6,y,color);
				}
				else
				{
					color = 0;
					if(land2.bit6 == 0)
					{
						putpixel(x*8+6,y,0);
					}
					else
					{
						putpixel(x*8+6,y,15);
					}
				}
				color = 2;
				if(land1.bit7 == 0)
				{
					putpixel(x*8+7,y,color);
				}
				else
				{
					color = 0;
					if(land2.bit7 == 0)
					{
						putpixel(x*8+7,y,0);
					}
					else
					{
						putpixel(x*8+7,y,15);
					}
				}
			}
		}
		fclose(fp1);
		fclose(fp2);
	}
}
