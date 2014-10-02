/* MANENV01.C: Manufacturing Program environment 1

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/
#include<stdio.h>
#include<dos.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include"popup.h"
#include"textbox.h"
#include"cursor.h"
#include"form.h"
#include"keyio.h"
#include"title.h"

#define MAXCHAR		1

#define TRUE		1
#define FALSE		!TRUE

/* function prototypes */
void write_char(char);
char read_char(char *);
int doMenu(int, POPUP *);
void Setup(char *);

/* global variables */

void main(void)
{
	int key = 0,
		end = 0,
		F10pressed = FALSE,
		x = 0;
	int ms[][2] =	{{1,3},
					 {7,9},
					 {13,19},
					 {23,31},
					 {35,45},
					 {49,53}};

	char attrib,
		name[8],
		title[] =	"25,1,Command Computer Services Ltd.",
		footer[] =	"1,1,F1-Help,"
					"11,1,F2-Accept,"
					"31,1,F8-Calculator,"
					"51,1,F9-Calander,"
					"71,1,F10-Menu",
		menu[] =	"A/R,A/P,Payroll,Inventory,"
					"Job costing,Setup",
		setuppop[] = "1,1,Change color,"
					"1,2,Set directory,"
					"1,3,Save settings,"
					"1,4,Restore defaults",
		endwindow[] = "Are you sure you want to exit? (Y/N)",
		helptext[] = " What do YOU want help for?";

	struct text_info original;

	POPUP	popend = {29,14,51,17,2,BLUE,BLUE,RED,RED,GREEN},
			pophelp = {5,5,34,7,1,GREEN,GREEN,RED,RED,YELLOW};

	MENUBAR	bar = {2,BLACK,LIGHTGRAY};

/*	Show_logo; */

	popend.text = endwindow;
	pophelp.text = helptext;
	bar.choice = menu;

	gettextinfo(&original);

	cursoff();

	Put_window(1,1,80,1,LIGHTBLUE,BLUE);
	clrscr();
	Show_form(title);

	Put_window(1,25,80,25,BLUE,LIGHTGRAY);
	clrscr();
	Show_form(footer);

	menubar(&bar);

	Put_window(1,3,80,24,YELLOW,BLUE);
	clrscr();

	while(!end)
	{
		key = Get_key();
		switch(key)
		{
			case RIGHT:	if(F10pressed)
						{
							x++;
							if(x>5)
								x = 0;
							menubar(&bar);
							attrib = WHITE + (BLACK << 4);
							Menu_hilite(ms[x][0],ms[x][1],bar.row,attrib);
							Put_window(1,3,80,24,YELLOW,BLUE);
							clrscr();
						}
						break;
			case LEFT:	if(F10pressed)
						{
							x--;
							if(x<0)
								x = 5;
							menubar(&bar);
							attrib = WHITE + (BLACK << 4);
							Menu_hilite(ms[x][0],ms[x][1],bar.row,attrib);
							Put_window(1,3,80,24,YELLOW,BLUE);
							clrscr();
						}
						break;
			case DOWN:  if(F10pressed)
						{
							switch(x)
							{
								case 0:	cprintf("A/R not working\r\n");
										break;
								case 1:	cprintf("A/P not working\r\n");
										break;
								case 2:	cprintf("Payroll not working\r\n");
										break;
								case 3:	cprintf("Inventory not working\r\n");
										break;
								case 4:	cprintf("J/C not working\r\n");
										break;
								case 5: Setup(setuppop);
										break;
							}
						}
						else
						{
							putchar(7);
						}
						break;
			case F1:	popShow(&pophelp);
						getch();
						popErase(&pophelp);
						break;
			case F2:	cprintf("Accept not working\r\n");
						break;
			case F8:	cprintf("Calculator not working\r\n");
						break;
			case F9:	cprintf("Calander not working\r\n");
						break;
			case F10:   if(!F10pressed)
						{
							menubar(&bar);
							attrib = WHITE + (BLACK << 4);
							Menu_hilite(ms[0][0],ms[0][1],bar.row,attrib);
							F10pressed = TRUE;
							x = 0;
						}
						else
						{
							menubar(&bar);
							F10pressed = FALSE;
							x = 0;
						}
						Put_window(1,3,80,24,YELLOW,BLUE);
						clrscr();
						break;
			case ALTR:	menubar(&bar);
						attrib = WHITE + (BLACK << 4);
						Menu_hilite(ms[0][0],ms[0][1],bar.row,attrib);
						x = 0;
						Put_window(1,3,80,24,YELLOW,BLUE);
						clrscr();
						break;
			case ALTP:	menubar(&bar);
						attrib = WHITE + (BLACK << 4);
						Menu_hilite(ms[1][0],ms[1][1],bar.row,attrib);
						x = 1;
						Put_window(1,3,80,24,YELLOW,BLUE);
						clrscr();
						break;
			case ALTY:	menubar(&bar);
						attrib = WHITE + (BLACK << 4);
						Menu_hilite(ms[2][0],ms[2][1],bar.row,attrib);
						x = 2;
						Put_window(1,3,80,24,YELLOW,BLUE);
						clrscr();
						break;
			case ALTI:	menubar(&bar);
						attrib = WHITE + (BLACK << 4);
						Menu_hilite(ms[3][0],ms[3][1],bar.row,attrib);
						x = 3;
						Put_window(1,3,80,24,YELLOW,BLUE);
						clrscr();
						break;
			case ALTJ:	menubar(&bar);
						attrib = WHITE + (BLACK << 4);
						Menu_hilite(ms[4][0],ms[4][1],bar.row,attrib);
						x = 4;
						Put_window(1,3,80,24,YELLOW,BLUE);
						clrscr();
						break;
			case ALTS:	menubar(&bar);
						attrib = WHITE + (BLACK << 4);
						Menu_hilite(ms[5][0],ms[5][1],bar.row,attrib);
						x = 5;
						Put_window(1,3,80,24,YELLOW,BLUE);
						clrscr();
						break;
			case ESC:
			case ALTX:	popShow(&popend);
						do
						{
							putchar(7);
							key = Get_key();
						} while(key != 'Y' && key != 'N');
						if(key == 'Y')
						{
							end = 1;
						}
						else
						{
							popErase(&popend);
						}
						break;
			default:    putchar(7);
						break;
		}
	}

	curson();
	Put_original(original);
	clrscr();
}

void write_char(char n)
{
	int x, y, bad, fini;
	FILE *fp;
	char name[12] = "",
		 *cname;

	x = 0;
	fini = 0;
	while(!fini || x == 8)
	{
		if(*cname >= 'A' && *cname <= 'Z')
		{
			name[x] = *cname;
			cname++;
			x++;
		}
		else
		{
			fini = 1;
		}
	}
	name[x] = '.';
	name[x+1] = 'd';
	name[x+2] = 'a';
	name[x+3] = 't';

	if(!(fp=fopen(name,"wb")))
	{
		cprintf("\nCannot open file.");
	}
	else
	{
		fwrite(&n,sizeof(char ),1,fp);
		fclose(fp);
	}
}

char read_char(char name[])
{
	FILE *fp;
	char n;
	int x;

	n = NULL;
	for(x=0;x<=12;x++)
	{
		if((name[x] < 'A' || name[x] > 'Z') &&
		   (name[x] < 'a' || name[x] > 'z') &&
		   (name[x] != '.'))
			name[x] = NULL;
	}

	if(!(fp=fopen(name,"rb")))
	{
		cprintf("\nCannot open file.");
	}
	else
	{
		fread(&n,sizeof(char),1,fp);
		fclose(fp);
	}
	return(n);
}

void Setup(char msg[])
{
	Put_window(45,3,61,6,GREEN,RED);
	clrscr();
	Show_form(msg);
}