/* ADVCHR7.C: Adventure character designer 7

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<time.h>
#include"form.h"
#include"keyio.h"

#define MAXCHAR		1

char CCSL[] = "Command Computer Services Ltd.";

typedef struct {
	char	name [10];		/* name of character */
	int		class;			/* character class */
	int		preq[2];		/* prerequisite and adjustments */
	int		str[2];			/* strength and adjustments */
	int		sma[4];			/* intelligence and adjustments */
	int		wis[2];			/* wisdom and adjustments */
	int 	dex[4];			/* dexterity and adjustments */
	int		con[2];			/* constitution and adjustments */
	int		cha[4];			/* charisma and adjustments */
	int		hp;				/* characters hit points */
	int		ac;				/* characters armor class */
	int 	algnmnt;		/* characters alignment */
	int		lvl;			/* characters level of experience */
	int		xp[2];			/* experience points and points needed to advance */
	int		coin[5];		/* coins in possesion: pp, gp, ep, sp, cp */
	int		hitac0;			/* roll required to hit armor class 0 */
}  character;

/* function prototypes */
int rnd(int);
int dice(int,int);
void create(void);
void display(character);
character keep(void);
character read_char(char *);
void write_char(character);

/* global variables */
int str, sma, wis, dex, con, cha, hp, gp, cc;

void main(void)
{
	character chr[MAXCHAR];
	int x, xpos, ypos, charnum = 0, counter;
	int fini;
	char y = 'n',
		choice = '0',
		name[8],
		form1[] = "25,1,Command Computer Services Ltd.",
		form2[] = "1,1,Do you wish to:,"
				  "1,2,1 - View character,"
				  "1,3,2 - Create character";



	struct text_info original;

	gettextinfo(&original);

	Put_window(1,1,80,1,BLUE,LIGHTGRAY);
	clrscr();
	Show_form(form1);

	Put_window(1,2,80,24,YELLOW,BLUE);
	clrscr();

	while(choice != '1' && choice != '2')
	{
		Show_form(form2);
		choice = getche();
		if(choice != '1' && choice != '2')
			putchar(7);
	}

	clrscr();

	if(choice == '1')
	{
		cprintf("Enter the characters name.\n\r");
		counter = Get_form_string(name, 8);
		name[counter] = '.';
		name[counter+1] = 'd';
		name[counter+2] = 'a';
		name[counter+3] = 't';

		chr[charnum] = read_char(name);
		if(chr[charnum].name[0] != NULL)
			display(chr[charnum]);
		getch();
	}
	else
	{
		randomize ();
		while(charnum < MAXCHAR)
		{
			while(y == 'n')
			{
				create();
				cprintf("\n\rDo you wish to keep this character? (y/n) ");
				y =' ';
				while(y != 'y' && y != 'n')
				{
					y = tolower(getch());
					if(y != 'y' && y != 'n')
						putchar(7);
				}
			}
			y = 'n';
			chr[charnum] = keep();
			write_char(chr[charnum]);
			charnum++;
		}
		window(1,2,80,24);
		for(x = 0; x < MAXCHAR; x++)
		{
			display(chr[x]);
			getch();
		}
	}
	Put_original(original);
	clrscr();
}

void create(void)
{
	int die = 1,
		times = 1,
		pre_req,
		dflt,
		change;
	char charclass,
		choice2,
		choice3;

	window(61,2,80,10);
	clrscr();
	cprintf("Fighter\n\r");
	cprintf("Thief\n\r");
	cprintf("Mage\n\r");
	cprintf("Cleric");

	window(1,11,80,24);
	clrscr();
	cprintf("\n\n\rPress the first letter of the class you wish to be: ");
	dflt = 0;
	while(!dflt)
	{
		charclass = tolower(getch());
		switch(charclass)
		{
			case 'c':   dflt = 1;
						cc = 1;
						cprintf("CLERIC");
						pre_req = 3;
						break;
			case 'f':	dflt = 1;
						cc = 4;
						cprintf("FIGHTER");
						pre_req = 1;
						break;
			case 'm':	dflt = 1;
						cc = 5;
						cprintf("MAGE");
						pre_req = 2;
						break;
			case 't':	dflt = 1;
						cc = 6;
						cprintf("THIEF");
						pre_req = 4;
						break;
			default :	putchar(7);
		}
	}

	window(1,2,60,10);
	clrscr();
	gotoxy(1,1);
	cprintf("Strength\n\r");
	cprintf("Intelligence\n\r");
	cprintf("Wisdom\n\r");
	cprintf("Dexterity\n\r");
	cprintf("Constitution\n\r");
	cprintf("Charisma");

	die = 6;
	while(pre_req)
	{
		str = dice(die,times) + 11;
		gotoxy(14,1);
		cprintf("%d",str);
		sma = dice(die,times) + 11;
		gotoxy(14,2);
		cprintf("%d",sma);
		wis = dice(die,times) + 11;
		gotoxy(14,3);
		cprintf("%d",wis);
		dex = dice(die,times) + 11;
		gotoxy(14,4);
		cprintf("%d",dex);
		con = dice(die,times) + 11;
		gotoxy(14,5);
		cprintf("%d",con);
		cha = dice(die,times) + 11;
		gotoxy(14,6);
		cprintf("%d",cha);

		switch(pre_req)
		{
			case 1:	if(str >= sma && str >= wis && str >= dex)
						pre_req = 0;
						break;
			case 2:	if(sma >= str && sma >= wis && sma >= dex)
						pre_req = 0;
						break;
			case 3:	if(wis >= str && wis >= sma && wis >= dex)
						pre_req = 0;
						break;
			case 4:	if(dex >= str && dex >= sma && dex >= wis)
						pre_req = 0;
						break;
		}
	}

	window(61,2,80,11);
	clrscr();
	window(1,11,80,24);
	change = 1;
	if(cc == 1 && wis == 18) change = 0;
	if(cc == 4 && str == 18) change = 0;
	if(cc == 5 && sma == 18) change = 0;
	if(cc == 6 && dex == 18) change = 0;
	if(change)
	{
		clrscr();
		cprintf("\n\n\rDo you wish to increase your prime requisite? (y/n) ");
		choice2 = tolower(getch());
		while(choice2 == 'y')
		{
			clrscr();
			cprintf("\n\rWhich do you want to decrease? ");
			if(cc != 4) cprintf("\n\r1 - Strength");
			if(cc != 5) cprintf("\n\r2 - Intelligence");
			if(cc != 1) cprintf("\n\r3 - Wisdom");
			if(cc != 6) cprintf("\n\r4 - Dexterity\n\r");
			choice3 = getch();
			switch(choice3)
			{
				case '1':
					if(cc == 4) putchar(7);
					else
					{
						if(str > 10)
						{
							str -= 2;
							switch(cc)
							{
								case 1: wis++; break;
								case 5: sma++; break;
								case 6: dex++; break;
							}
							if(cc == 1 && wis == 18) change = 0;
							if(cc == 5 && sma == 18) change = 0;
							if(cc == 6 && dex == 18) change = 0;
						}
					}
					break;
				case '2':
					if(cc == 5) putchar(7);
					else
					{
						if(sma > 10)
						{
							sma -=2;
							switch(cc)
							{
								case 1: wis++; break;
								case 4: str++; break;
								case 6: dex++; break;
							}
							if(cc == 1 && wis == 18) change = 0;
							if(cc == 4 && str == 18) change = 0;
							if(cc == 6 && dex == 18) change = 0;
						}
					}
					break;
				case '3':
					if(cc == 1) putchar(7);
					else
					{
						if(wis > 10)
						{
							wis -=2;
							switch(cc)
							{
								case 4: str++; break;
								case 5: sma++; break;
								case 6: dex++; break;
							}
							if(cc == 4 && str == 18) change = 0;
							if(cc == 5 && sma == 18) change = 0;
							if(cc == 6 && dex == 18) change = 0;
						}
					}
					break;
				case '4':
					if(cc == 6) putchar(7);
					else
					{
						if(dex > 10)
						{
							dex -=2;
							switch(cc)
							{
								case 1: wis++; break;
								case 4: str++; break;
								case 5: sma++; break;
							}
							if(cc == 1 && wis == 18) change = 0;
							if(cc == 4 && str == 18) change = 0;
							if(cc == 5 && sma == 18) change = 0;
						}
					}
					break;
				default: putchar(7);
			}
			window(1,2,60,10);
			clrscr();
			if(cc == 1) cprintf("Cleric");
			if(cc == 4) cprintf("Fighter");
			if(cc == 5) cprintf("Mage");
			if(cc == 6) cprintf("Thief");
			cprintf("\n\n\rStrength     %d",str);
			cprintf("\n\rIntelligence %d",sma);
			cprintf("\n\rWisdom       %d",wis);
			cprintf("\n\rDexterity    %d",dex);
			cprintf("\n\rConstitution %d",con);
			cprintf("\n\rCharisma     %d",cha);
			window(1,11,80,24);
			if(change)
			{
				clrscr();
				cprintf("\n\n\rAgain? (y/n)");
				choice2 = tolower(getch());
			}
			else
				choice2 = 'n';
		}
	}
	cprintf("\n\n\rEXP   0");
	hp = 0;
	while(hp < 4)
	{
		switch(cc)
		{
			case 1: hp = dice(6,1); break;
			case 4: hp = dice(8,1); break;
			case 5: hp = dice(4,1); break;
			case 6: hp = dice(4,1); break;
		}
		switch(con)
		{
			case 13:
			case 14:
			case 15: hp +=  1; break;
			case 16:
			case 17: hp +=  2; break;
			case 18: hp +=  3; break;
		}
	}
	clrscr();
	cprintf("\n\rHP    %d",hp);
	gp = dice(6,3)*100;
	cprintf("\n\rGP    %d",gp);
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

void display(character n)
{
	clrscr();
	gotoxy(1,1);
	cprintf("Name:  %s",n.name);
	gotoxy(1,2);
	cprintf("Class: ");
	switch(n.class)
	{
		case 1: cprintf("Cleric"); break;
		case 4: cprintf("Fighter"); break;
		case 5: cprintf("Magic User"); break;
		case 6: cprintf("Thief"); break;
	}
	gotoxy(1,4);
	cprintf("Strength      %2d",n.str[0]);
	gotoxy(1,5);
	cprintf("Intelligence  %2d",n.sma[0]);
	gotoxy(1,6);
	cprintf("Wisdom        %2d",n.wis[0]);
	gotoxy(1,7);
	cprintf("Dexterity     %2d",n.dex[0]);
	gotoxy(1,8);
	cprintf("Constitution  %2d",n.con[0]);
	gotoxy(1,9);
	cprintf("Charisma      %2d",n.cha[0]);
	gotoxy(25,1);
	cprintf("Alignment       ");
	switch(n.algnmnt)
	{
		case 1: cprintf("   Good"); break;
		case 2: cprintf("Neutral"); break;
		case 3: cprintf("   Evil"); break;
	}
	gotoxy(25,2);
	cprintf("Level                %2d",n.lvl);
	gotoxy(25,4);
	cprintf("Hit Points          %3d",n.hp);
	gotoxy(25,5);
	cprintf("Armor Class          %2d",n.ac);
	gotoxy(25,6);
	cprintf("Experience        %5d",n.xp[0]);
	gotoxy(25,7);
	cprintf("XP for next level %5d",n.xp[1]);
	gotoxy(25,8);
	cprintf("Gold Pieces     %7d",n.coin[1]);
	gotoxy(25,9);
	cprintf("THAC0                %d",n.hitac0);
}

character keep(void)
{
	int x;
	int counter,
		counter1;
	char temp1[2],
		 temp2[1];
	character n;

	clrscr();
	cprintf("Enter a name for this character (max 10 characters).\n\r");
	counter1 = 0;
	while(counter1 < 10)
	{
		n.name[counter1] = toupper(getche());
		if(n.name[counter1] == 13)
		{
			n.name[counter1] = NULL;
			counter1 = 11;
		}
		else
		{
			counter1++;
		}
	}
	n.class = cc;
	switch(n.class)
	{
		case  1: n.preq[0] = wis; break;
		case  4: n.preq[0] = str; break;
		case  5: n.preq[0] = sma; break;
		case  6: n.preq[0] = dex; break;
		default: n.preq[0] =   0;
	}
	switch(n.preq[0])
	{
		case 13:
		case 14:
		case 15: n.preq[1] =   5; break;
		case 16:
		case 17:
		case 18: n.preq[1] =  10; break;
		default: n.preq[1] =   0;
	}
	n.str[0] = str;
	switch(n.str[0])
	{
		case 13:
		case 14:
		case 15: n.str[1] =  1; break;
		case 16:
		case 17: n.str[1] =  2; break;
		case 18: n.str[1] =  3; break;
		default: n.str[1] =  0;
	}
	n.sma[0] = sma;
	switch(n.sma[0])
	{
		case 13:
		case 14:
		case 15: counter = 1; break;
		case 16:
		case 17: counter = 2; break;
		case 18: counter = 3; break;
		default: counter = 0;
	}
	if(counter)
	{
		for(x = 0; x < counter; x++)
		{
			clrscr();
			cprintf("Press the number corrisponding to the language you");
			cprintf("\n\rwish to speak.\n\n\r");
			cprintf("1. Bugbear       8. Gnome      15. Medusa\n\r");
			cprintf("2. Doppleganger  9. Goblin     16. Minotaur\n\r");
			cprintf("3. Dragon       10. Halfling   17. Ogre\n\r");
			cprintf("4. Dwarvish     11. Harpy      18. Orc\n\r");
			cprintf("5. Elvish       12. Hobgoblin  19. Pixie\n\r");
			cprintf("6. Gargoyle     13. Kobold     20. Human dialect\n\r");
			cprintf("7. Gnoll        14. Lizard Man\n\r");
			counter1 = 0;
			while(counter1 < 2)
			{
				temp1[counter1] = toupper(getche());
				if(temp1[counter1] == 13)
				{
					temp1[counter1] = temp1[counter1-1];
					temp1[counter1-1] = '0';
					counter1 = 3;
				}
				else
				{
					counter1++;
				}
			}
			n.sma[counter] = atoi(temp1);
		}
	}
	n.wis[0] = wis;
	switch(n.wis[0])
	{
		case 13:
		case 14:
		case 15: n.wis[1] =  1; break;
		case 16:
		case 17: n.wis[1] =  2; break;
		case 18: n.wis[1] =  3; break;
		default: n.wis[1] =  0;
	}
	n.dex[0] = dex;
	switch(n.dex[0])
	{
		case 13:
		case 14:
		case 15: n.dex[1]= 1; n.dex[2]=-1; n.dex[3]= 1; break;
		case 16:
		case 17: n.dex[1]= 2; n.dex[2]=-2; n.dex[3]= 1; break;
		case 18: n.dex[1]= 3; n.dex[2]=-3; n.dex[3]= 2; break;
		default: n.dex[1]= 0; n.dex[2]= 0; n.dex[3]= 0;
	}
	n.con[0] = con;
	switch(n.con[0])
	{
		case 13:
		case 14:
		case 15: n.con[1] =  1; break;
		case 16:
		case 17: n.con[1] =  2; break;
		case 18: n.con[1] =  3; break;
		default: n.con[1] =  0;
	}
	n.cha[0] = cha;
	switch(n.cha[0])
	{
		case 13:
		case 14:
		case 15: n.cha[1]= 1; n.cha[2]=5; n.cha[3]= 8; break;
		case 16:
		case 17: n.cha[1]= 2; n.cha[2]=6; n.cha[3]= 9; break;
		case 18: n.cha[1]= 3; n.cha[2]=7; n.cha[3]=10; break;
		default: n.cha[1]= 0; n.cha[2]=4; n.cha[3]= 7;
	}
	n.hp = hp;
	n.ac = n.dex[2] + 9;
	n.algnmnt = 0;
	while(!n.algnmnt)
	{
		clrscr();
		cprintf("Enter your characters allignment.");
		cprintf("\n\r1 - Good");
		cprintf("\n\r2 - Neutral");
		cprintf("\n\r3 - Evil\n\r");
		temp2[0] = getche();
		n.algnmnt = atoi(temp2);
		if(n.algnmnt!=1 && n.algnmnt!=2 && n.algnmnt!=3)
		{
			n.algnmnt = 0;
			putchar(7);
		}
	}
	n.lvl = 1;
	n.xp[0] = 0;
	switch(n.class)
	{
		case  1: n.xp[1] = 1500; break;
		case  4: n.xp[1] = 2000; break;
		case  5: n.xp[1] = 2500; break;
		case  6: n.xp[1] = 1200; break;
	}

	n.coin[0] = 0;
	n.coin[1] = gp;
	n.coin[2] = 0;
	n.coin[3] = 0;
	n.coin[4] = 0;
	n.hitac0 = 19;
	return(n);
}

int rnd(int number)
{
	/* selects a random number between 1 and 20 */
	number++;
/*	srand((unsigned) random(5000));*/
	return(random(number));
}

void write_char(character n)
{
	int x, y, bad, fini;
	FILE *fp;
	char name[12] = "",
		 *cname;

	cname = &n.name[0];
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
		fwrite(&n,sizeof(character),1,fp);
		fclose(fp);
	}
}

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
		cprintf("\nCannot open file.");
	}
	else
	{
		fread(&n,sizeof(character),1,fp);
		fclose(fp);
	}
	return(n);
}
