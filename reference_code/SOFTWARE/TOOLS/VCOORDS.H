/* VCOORDS.H:  Header for implementing 800 x 600 virtual
					display area in Turbo C graphics

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/

#define VH 600							/* height */
#define VW 800							/* width  */

/* GLOBALS DEFINED IN LIBRARY, EXTERNALLY VISIBLE */
extern int    px, py;               /* phisical x and y width       */
extern double xf, yf;               /* x and y translation factors  */

void setFactors (void);             /* compute translation factors  */
int dx (int vx);                    /* translate virt x to device x */
int dy (int vy);                    /* translate virt y to device y */