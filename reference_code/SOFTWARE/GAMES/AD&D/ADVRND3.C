/* ADVRND3.C: Adventure randomizer number 3

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

typedef struct {
	char	name [10];		/* name of character */
	int		class;			/* character class */
	int 	nat[4];			/* native languages */
	int		preq[2];		/* prerequisite and adjustments */
	int		str[2];			/* strength and adjustments */
	int		sma[4];			/* intelligence and adjustments */
	int		wis[2];			/* wisdom and adjustments */
	int 	dex[4];			/* dexterity and adjustments */
	int		con[2];			/* constitution and adjustments */
	int		cha[4];			/* charisma and adjustments */
	int		hp;				/* characters hit points */
	int		ac;				/* characters armor class */
	int 	algnmnt;		/* characters allignment */
	int		lvl;			/* characters level of experience */
	int		xp[2];			/* experience points and points needed to advance */
	int		coin[5];		/* coins in possesion: pp, gp, ep, sp, cp */
	int		hitac0;			/* roll required to hit armor class 0 */
}  character;

/* function prototypes */
int rnd(int);
int dice(int,int);
void create(void);
void display(int);
void keep(int);

/* global variables */
int str, sma, wis, dex, con, cha, hp, gp, cc;
int counter = 10;
int die = 6, times = 3;
int dflt;
int change;
char choice2;
char choice3;
char yn;
char charclass;
character chr[8];

main()
{
int x;
char y = 'n';
char charnum = 0;

	randomize ();
	while(charnum < 8) {
		while(y == 'n') {
			create();
			printf("\nDo you wish to keep this character? (y/n)");
			y =' ';
			while(y != 'y' && y != 'n') {
				y = tolower(getch());
				if(y != 'y' && y != 'n') putchar(7);
			}
		}
		y = 'n';
		keep(charnum);
		charnum++;
	}
	for(x = 0; x < 8; x++) {
		display(x);
		getch();
	}
}

void create(void)
{
	while (counter>2) {
		counter = 0;
		str = dice(die,times);
		sma = dice(die,times);
		wis = dice(die,times);
		dex = dice(die,times);
		con = dice(die,times);
		cha = dice(die,times);
		if (str < 10) counter ++;
		if (str <  6) counter ++;
		if (sma < 10) counter ++;
		if (sma <  6) counter ++;
		if (wis < 10) counter ++;
		if (wis <  6) counter ++;
		if (dex < 10) counter ++;
		if (dex <  6) counter ++;
		if (con < 10) counter ++;
		if (con <  6) counter ++;
		if (cha < 10) counter ++;
		if (cha <  6) counter ++;
		if (str > 16) counter --;
		if (str > 17) counter --;
		if (sma > 16) counter --;
		if (sma > 17) counter --;
		if (wis > 16) counter --;
		if (wis > 17) counter --;
		if (dex > 16) counter --;
		if (dex > 17) counter --;
		if (con > 16) counter --;
		if (con > 17) counter --;
		if (cha > 16) counter --;
		if (cha > 17) counter --;
	}
	clrscr();
	printf("start\n\n");
	printf("Strength      %d\n",str);
	printf("Intelligence  %d\n",sma);
	printf("Wisdom        %d\n",wis);
	printf("Dexterity     %d\n",dex);
	printf("Constitution  %d\n",con);
	printf("Charisma      %d",  cha);
	printf("\n\n");
	if(str > sma && str > wis && str > dex)
		printf("You'ld be better off as a FIGHTER");
	if(sma > str && sma > wis && sma > dex)
		printf("You'ld be better off as a MAGIC USER");
	if(wis > str && wis > sma && wis > dex)
		printf("You'ld be better off as a CLERIC");
	if(dex > str && dex > sma && dex > wis)
		printf("You'ld be better off as a THIEF");
	printf("\n\nPress the first letter of the class you wish to be");
	dflt = 0;
	while(!dflt) {
		charclass = getch();
		charclass = tolower(charclass);
		switch(charclass) {
		case 'c': dflt = 1; cc = 1; break;
		case 'd': dflt = 1; cc = 2; break;
		case 'e': dflt = 1; cc = 3; break;
		case 'f': dflt = 1; cc = 4; break;
		case 'm': dflt = 1; cc = 5; break;
		case 't': dflt = 1; cc = 6; break;
		case 'h': dflt = 1; cc = 7; break;
		default : putchar(7);
		}
		if(cc == 2 && con < 9) {
			dflt = 0;
			printf("\nYou cannot be a Dwarf with a low Constitution.");
			printf("\nPlease choose again.");
		}
		if(cc == 3 && sma < 9) {
			dflt = 0;
			printf("\nYou cannot be an Elf with a low Intelligence.");
			printf("\nPlease choose again.");
		}
		if(cc == 7 && (dex < 9 || con < 9)) {
			dflt = 0;
			printf("\nYou cannot be a Halfling with low Dexterity or");
			printf("\nConstitution.  Please choose again.");
		}
	}
	change = 1;
	if(cc == 1 && wis == 18) change = 0;
	if(cc == 2 && str == 18) change = 0;
	if(cc == 4 && str == 18) change = 0;
	if(cc == 5 && sma == 18) change = 0;
	if(cc == 6 && dex == 18) change = 0;
	if(cc != 3 && cc != 7 && change) {
		printf("\n\nDo you wish to increase your prime requisite? (y/n)");
		choice2 = tolower(getch());
		while(choice2 == 'y') {
			printf("\nWhich do you want to decrease?");
			if(cc!=4 && cc!=2) printf("\n1 - Strength");
			if(cc != 5) printf("\n2 - Intelligence");
			if(cc != 1) printf("\n3 - Wisdom");
			if(cc != 6) printf("\n4 - Dexterity");
			choice3 = getch();
			switch(choice3) {
			case '1':
				if(cc == 4 || cc == 2) putchar(7);
				else {
					if(str > 10) {
						str -= 2;
						switch(cc) {
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
				else {
					if(sma > 10) {
						sma -=2;
						switch(cc) {
						case 1: wis++; break;
						case 2: str++; break;
						case 4: str++; break;
						case 6: dex++; break;
						}
						if(cc == 1 && wis == 18) change = 0;
						if(cc == 2 && str == 18) change = 0;
						if(cc == 4 && str == 18) change = 0;
						if(cc == 6 && dex == 18) change = 0;
					}
				}
				break;
			case '3':
				if(cc == 1) putchar(7);
				else {
					if(wis > 10) {
						wis -=2;
						switch(cc) {
						case 2: str++; break;
						case 4: str++; break;
						case 5: sma++; break;
						case 6: dex++; break;
						}
						if(cc == 2 && str == 18) change = 0;
						if(cc == 4 && str == 18) change = 0;
						if(cc == 5 && sma == 18) change = 0;
						if(cc == 6 && dex == 18) change = 0;
					}
				}
				break;
			case '4':
				if(cc == 6) putchar(7);
				else {
					if(dex > 10) {
						dex -=2;
						switch(cc) {
						case 1: wis++; break;
						case 2: str++; break;
						case 4: str++; break;
						case 5: sma++; break;
						}
						if(cc == 1 && wis == 18) change = 0;
						if(cc == 2 && str == 18) change = 0;
						if(cc == 4 && str == 18) change = 0;
						if(cc == 5 && sma == 18) change = 0;
					}
				}
				break;
			default: putchar(7);
			}
			clrscr();
			if(cc == 1) printf("Cleric");
			if(cc == 2) printf("Dwarf");
			if(cc == 3) printf("Elf");
			if(cc == 4) printf("Fighter");
			if(cc == 5) printf("Magic User");
			if(cc == 6) printf("Thief");
			if(cc == 7) printf("Halfling");
			printf("\n\nStrength     %d",str);
			printf("\nIntelligence %d",sma);
			printf("\nWisdom       %d",wis);
			printf("\nDexterity    %d",dex);
			printf("\nConstitution %d",con);
			printf("\nCharisma     %d",cha);
			if(change) {
				printf("\n\nAgain? (y/n)");
				choice2 = tolower(getch());
			}
			else
				choice2 = 'n';
		}
	}
	printf("\n\nEXP   0");
	hp = 0;
	while(hp < 3) {
		switch(cc) {
		case 1: hp = dice(6,1); break;
		case 2: hp = dice(8,1); break;
		case 3: hp = dice(6,1); break;
		case 4: hp = dice(8,1); break;
		case 5: hp = dice(4,1); break;
		case 6: hp = dice(4,1); break;
		case 7: hp = dice(6,1); break;
		}
		switch(con) {
		case  3: hp += -3; break;
		case  4:
		case  5: hp += -2; break;
		case  6:
		case  7:
		case  8: hp += -1; break;
		case 13:
		case 14:
		case 15: hp +=  1; break;
		case 16:
		case 17: hp +=  2; break;
		case 18: hp +=  3; break;
		}
	}
	printf("\nHP    %d",hp);
	gp = dice(6,3)*10;
	if(gp<90)
		gp = 90;
	printf("\nGP    %d",gp);
	counter = 10;
}

int dice(int die, int times)
{
	int x, z, newsum = 0;

	if(die!=100) {
		for(x=0;x<times;x++) {
			do {
				z = rnd(die);
			}
			while(z==0);
			newsum += z;
		}
		return(newsum);
	}
	else
		return(rnd(die));
}

void display(int n)
{
	clrscr();
	printf("%s             %d",chr[n].name,chr[n].class);
	printf("\n\nSTR  %d              hp %d",chr[n].str[0],chr[n].hp);
	printf("\nINT  %d",chr[n].sma[0]);
	printf("\nWIS  %d              ac %d",chr[n].wis[0],chr[n].ac);
	printf("\nDEX  %d",chr[n].dex[0]);
	printf("\nCON  %d              al %d",chr[n].con[0],chr[n].algnmnt);
	printf("\nCHA  %d",chr[n].cha[0]);
	printf("\n\nLVL  %d      XP  %d      GP %d",chr[n].lvl,
															  chr[n].xp[0],
															  chr[n].coin[1]);
	printf("\nNeed %dXP for next level",chr[n].xp[1]);
	printf("\n\nTo hit AC 0  %d",chr[n].hitac0);
}

void keep(int n)
{
	int x;
	int length;
	int counter;

	clrscr();
	printf("Enter a name for this character (max 10 characters).");
	length = 11;
	while(length > 10) {
		scanf("%s",chr[n].name);
		chr[n].name[10] = strupr(chr[n].name);
		length = strlen(chr[n].name);
	}
	chr[n].class = cc;
	switch(chr[n].class) {
	case  1:	chr[n].preq[0] = wis; break;
	case  2: chr[n].preq[0] = str; break;
	case  4: chr[n].preq[0] = str; break;
	case  5: chr[n].preq[0] = sma; break;
	case  6: chr[n].preq[0] = dex; break;
	default: chr[n].preq[0] =   0;
	}
	switch(chr[n].preq[0]) {
	case  3:
	case  4:
	case  5: chr[n].preq[1] = -20; break;
	case  6:
	case  7:
	case  8: chr[n].preq[1] = -10; break;
	case 13:
	case 14:
	case 15: chr[n].preq[1] =   5; break;
	case 16:
	case 17:
	case 18: chr[n].preq[1] =  10; break;
	default: chr[n].preq[1] =   0;
	}
	if(chr[n].class == 3) {
		if(str > 12 && sma > 12) chr[n].preq[1] =  5;
		if(str > 12 && sma > 15) chr[n].preq[1] = 10;
	}
	if(chr[n].class == 7) {
		if(str > 12 || dex > 12) chr[n].preq[1] =  5;
		if(str > 12 && dex > 12) chr[n].preq[1] = 10;
	}
	chr[n].str[0] = str;
	switch(chr[n].str[0]) {
	case  3: chr[n].str[1] = -3; break;
	case  4:
	case  5: chr[n].str[1] = -2; break;
	case  6:
	case  7:
	case  8: chr[n].str[1] = -1; break;
	case 13:
	case 14:
	case 15: chr[n].str[1] =  1; break;
	case 16:
	case 17: chr[n].str[1] =  2; break;
	case 18: chr[n].str[1] =  3; break;
	default: chr[n].str[1] =  0;
	}
	chr[n].sma[0] = sma;
	switch(chr[n].sma[0]) {
	case 13:
	case 14:
	case 15: counter = 1; break;
	case 16:
	case 17: counter = 2; break;
	case 18: counter = 3; break;
	default: counter = 0;
	}
	if(counter) {
		for(x = 0; x < counter; x++) {
			clrscr();
			printf("Press the number corrisponding to the language you");
			printf("\nwish to speak.\n\n");
			printf("1. Bugbear       8. Gnome      15. Medusa\n");
			printf("2. Doppleganger  9. Goblin     16. Minotaur\n");
			printf("3. Dragon       10. Halfling   17. Ogre\n");
			printf("4. Dwarvish     11. Harpy      18. Orc\n");
			printf("5. Elvish       12. Hobgoblin  19. Pixie\n");
			printf("6. Gargoyle     13. Kobold     20. Human dialect\n");
			printf("7. Gnoll        14. Lizard Man\n");
			scanf("%d",&chr[n].sma[x+1]);
		}
	}
	if(chr[n].class == 2) {
		chr[n].nat[0] =  4;
		chr[n].nat[1] =  8;
		chr[n].nat[2] = 13;
		chr[n].nat[3] =  9;
	}
	if(chr[n].class == 3) {
		chr[n].nat[0] =  5;
		chr[n].nat[1] = 18;
		chr[n].nat[2] = 12;
		chr[n].nat[3] =  7;
	}
	chr[n].wis[0] = wis;
	switch(chr[n].wis[0]) {
	case  3: chr[n].wis[1] = -3; break;
	case  4:
	case  5: chr[n].wis[1] = -2; break;
	case  6:
	case  7:
	case  8: chr[n].wis[1] = -1; break;
	case 13:
	case 14:
	case 15: chr[n].wis[1] =  1; break;
	case 16:
	case 17: chr[n].wis[1] =  2; break;
	case 18: chr[n].wis[1] =  3; break;
	default: chr[n].wis[1] =  0;
	}
	chr[n].dex[0] = dex;
	switch(chr[n].dex[0]) {
	case  3:chr[n].dex[1]=-3;chr[n].dex[2]= 3;chr[n].dex[3]=-2;break;
	case  4:
	case  5:chr[n].dex[1]=-2;chr[n].dex[2]= 2;chr[n].dex[3]=-1;break;
	case  6:
	case  7:
	case  8:chr[n].dex[1]=-1;chr[n].dex[2]= 1;chr[n].dex[3]=-1;break;
	case 13:
	case 14:
	case 15:chr[n].dex[1]= 1;chr[n].dex[2]=-1;chr[n].dex[3]= 1;break;
	case 16:
	case 17:chr[n].dex[1]= 2;chr[n].dex[2]=-2;chr[n].dex[3]= 1;break;
	case 18:chr[n].dex[1]= 3;chr[n].dex[2]=-3;chr[n].dex[3]= 2;break;
	default:chr[n].dex[1]= 0;chr[n].dex[2]= 0;chr[n].dex[3]= 0;
	}
	chr[n].con[0] = con;
	switch(chr[n].con[0]) {
	case  3: chr[n].con[1] = -3; break;
	case  4:
	case  5: chr[n].con[1] = -2; break;
	case  6:
	case  7:
	case  8: chr[n].con[1] = -1; break;
	case 13:
	case 14:
	case 15: chr[n].con[1] =  1; break;
	case 16:
	case 17: chr[n].con[1] =  2; break;
	case 18: chr[n].con[1] =  3; break;
	default: chr[n].con[1] =  0;
	}
	chr[n].cha[0] = cha;
	switch(chr[n].cha[0]) {
	case  3:chr[n].cha[1]=-2;chr[n].cha[2]=1;chr[n].cha[3]= 4;break;
	case  4:
	case  5:chr[n].cha[1]=-1;chr[n].cha[2]=2;chr[n].cha[3]= 5;break;
	case  6:
	case  7:
	case  8:chr[n].cha[1]=-1;chr[n].cha[2]=3;chr[n].cha[3]= 6;break;
	case 13:
	case 14:
	case 15:chr[n].cha[1]= 1;chr[n].cha[2]=5;chr[n].cha[3]= 8; break;
	case 16:
	case 17:chr[n].cha[1]= 2;chr[n].cha[2]=6;chr[n].cha[3]= 9; break;
	case 18:chr[n].cha[1]= 3;chr[n].cha[2]=7;chr[n].cha[3]=10; break;
	default:chr[n].cha[1]= 0;chr[n].cha[2]=4;chr[n].cha[3]= 7;
	}
	chr[n].hp = hp;
	chr[n].ac = chr[n].dex[2] + 9;
	chr[n].algnmnt = 0;
	while(!chr[n].algnmnt) {
		clrscr();
		printf("Enter your characters allignment.");
		printf("\n1 - Lawful");
		printf("\n2 - Neutral");
		printf("\n3 - Chaotic");
		scanf("%d",&chr[n].algnmnt);
		if(chr[n].algnmnt!=1 && chr[n].algnmnt!=2 && chr[n].algnmnt!=3) {
			chr[n].algnmnt = 0;
			putchar(7);
		}
	}
	chr[n].lvl = 1;
	chr[n].xp[0] = 0;
	switch(chr[n].class) {
	case  1: chr[n].xp[1] = 1500; break;
	case  2: chr[n].xp[1] = 2200; break;
	case  3: chr[n].xp[1] = 4000; break;
	case  4: chr[n].xp[1] = 2000; break;
	case  5: chr[n].xp[1] = 2500; break;
	case  6: chr[n].xp[1] = 1200; break;
	case  7: chr[n].xp[1] = 2000; break;
	}

	chr[n].coin[0] = 0;
	chr[n].coin[1] = gp;
	chr[n].coin[2] = 0;
	chr[n].coin[3] = 0;
	chr[n].coin[4] = 0;
	chr[n].hitac0 = 19;
}

int rnd(int number)
{
	/* selects a random number between 1 and 20 */
	number++;
/*	srand((unsigned) random(5000));*/
	return(random(number));
}