/* GRAFX2.H: graphics header file for graphic functions, structs, etc

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

	Date modified:	June 17, 1993
	Modified by:	Kevin Rattai
	Modifications:	Added functions to manipulate objects

*/

#ifndef BON
#define BOF   0
#define BON   !BOF
#endif
#define b(x)      (x/8)
#define pixup(x)  (x++)
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
int    bittoscr(bfield, int, int, int);		   /* puts int to screen */
int	 bndchck(int, int);					/* checks boundary violation */
void   drawarc(void);								 /* draws an arc */
void   drawbybit(void);					   /* illuminates pix by pix */
void   drawcircle(void);						   /* draws a circle */
void   drawline(void);								 /* draws a line */
void   drawpie(void);								  /* draws a pie */
void   drawrectangle(void);						/* draws a rectangle */
void   drawsquare(void);						   /* draws a square */
void   drawtriangle(void);						 /* draws a triangle */
int    drive(int);					 /* opens/closes graphics driver */
/* int getspcl(void);		/* returns number to special gr function */
void   help(void);						   /* shows HOT keys to user */
bfield initbyt(bfield);			 /* initializes bfield array to NULL */
void   movepicture(void);  /* moves picture from one area to another */
int    pixmove(int, int);/* pixel repositioned after cursor movement */
void   rotatepicture(void);			  /* turns picture around Y axis */
void   savepicture(void);					/* saves picture to disk */
bfield scrtobit(int, bfield);				   /* puts screen to int */
bfield subbit(int, bfield);					 /* removes bit from int */

void keyboard(void);			/* examines keyboard for key pressed */
void quit_pgm(void);					/* quits program via exit(0) */
void calc_3d(void);		  /* calculates position of object in 3space */
void rotation(void);					 /* rotates object in 3space */
void windows(void);					/* claculates object onto screen */
void graphics_setup(void);				 /* sets up graphics display */
void newpos(void);				/* determines new position of object */