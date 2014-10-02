/* BUTTON.H: Button header file

	Created by:		Kevin Rattai
	Dated created:	May 1, 1993

	Modified:		May 23, 1993
	Description:	Added remove_button() and clearbutton() prototypes
	modified by:	Kevin Rattai

*/

#include<stdio.h>
#include<dos.h>

#define SANSSERIF		3

typedef struct {
	int number;
	int top;
	int left;
	int bot;
	int right;
	char msg[40];
} SCRNBUTTON;

/* function prototypes */
void defbutton(int left, int top, char msg[]);
void defplate(int left, int top, char msg[]);
void putbutton(SCRNBUTTON *but);
void buttonon(int x);
void buttonoff(int x);
void showbounds(int tlcolor,int color,
				int left, int top, int right, int bot);
void putplate(SCRNBUTTON *but);
int checkhit(int z, int x, int y);
int platehit(int z, int x, int y);
void clearplates(void);
void remplate(SCRNBUTTON *but);
void remove_button(int num);
void clearbutton(SCRNBUTTON *but);