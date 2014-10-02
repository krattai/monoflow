/* RAT.H: Mouse header file

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/
#include<stdio.h>
#include<dos.h>

#define LBUTTON		1
#define RBUTTON		2
#define LRBUTTON	3
#define CBUTTON		4
#define LCBUTTON	5
#define RCBUTTON	6
#define LCRBUTTON	7

#define PRESS		-1
#define RELEASE		-2
#define MOVED		-3
#define HELD		-5

typedef struct {
	int buttons;
	int horiz;
	int verti;
} RATINFO;

/* function prototypes */
void cls(void);
void goxy(int,int);
void initrat(RATINFO *ratinfo);
void raton(void);
void ratoff(void);
int getratinput(RATINFO *ratinfo);
void resetrat(RATINFO *ratinfo);
void ROMBasic(void);
void coldboot(void);
void warmboot(void);