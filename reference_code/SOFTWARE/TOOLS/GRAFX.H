/* GRAFX.H: graphics header file for graphic functions, structs, etc

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/

#ifndef BON						/************************************/
#define BOF   0            /** don't remove these definitions **/
#define BON   !BOF         /************************************/
#endif
#define HOME 71				/* all of these could/should be defined */
#define UP   72				/* in the main program interp function  */
#define PGUP 73
#define LEFT 75
#define RITE 77
#define END  79
#define DOWN 80
#define PGDN 81
#define INS  82
#define b(x)      (x/8)		/* these should be left 'cause they */
#define pixup(x)  (x++)		/* may be used in both sub programs */
#define pixdn(x)  (x--)
#define update(x) (x++)
#define bitpos(x) (x - (x / 8 * 8))		/* or x % 8 (mod) */

typedef struct {
	unsigned bit8 : 1;
	unsigned bit7 : 1;
	unsigned bit6 : 1;
	unsigned bit5 : 1;
	unsigned bit4 : 1;
	unsigned bit3 : 1;
	unsigned bit2 : 1;
	unsigned bit1 : 1;
}  bfield; /* in programs use typedef bfield bcode[]; */

/* Some basic functions */
int    bittoscr(bfield, int, int, int);        /* puts int to screen */
int	 bndchck(int, int);					  /* checks boundary violation */
void   drawarc(void);                                /* draws an arc */
void   drawbybit(void);                    /* illuminates pix by pix */
void   drawcircle(void);                           /* draws a circle */
void   drawline(void);                               /* draws a line */
void   drawpie(void);                                 /* draws a pie */
void   drawrectangle(void);                     /* draws a rectangle */
void   drawsquare(void);                           /* draws a square */
void   drawtriangle(void);                       /* draws a triangle */
int    drive(int);						 /* opens/closes graphics driver */
/* int getspcl(void);		 /* returns number to special gr function */
void   help(void);								 /* shows HOT keys to user */
bfield initbyt(bfield);				/* initializes bfield array to NULL */
void   movepicture(void);  /* moves picture from one area to another */
int    pixmove(int, int);/* pixel repositioned after cursor movement */
void   rotatepicture(void);           /* turns picture around Y axis */
void   savepicture(void);                   /* saves picture to disk */
bfield scrtobit(int, bfield);                  /* puts screen to int */
bfield subbit(int, bfield);                  /* removes bit from int */