/* GRFXFORM.H: Graphics form header

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

	Modification:	May 22, 1993
	Description:	Added display form type and updated showform() and
						eraseform() to include display form type.
	Modified by:	Kevin Rattai

	Modification:	May 23, 1993
	Description:	Added new prototype restore_screen() for displaying
						logo.
	Modified by:	Kevin Rattai

*/

/* CCSL colors */
#define CCSLBLACK		0
#define CCSLLTBLUE		1
#define CCSLDKBLUE		2
#define CCSLGREY		3
#define CCSLWHITE		4
#define CCSLCHARCOAL	5

/* Display form type */
#define WITHOUT			0
#define LLEFT			1
#define LABOVE			2

#define MAXFIELDS		20

/* define structures */
typedef struct {
	int top;
	int left;
	int bot;
	int right;
	char label[15];	
} GRFXFLD;

typedef struct {
	int number;
	GRFXFLD fld[MAXFIELDS];
} GRFXFORM;

/* function prototypes */
void changecolor(int, int, int, int);
void setccslpalette(void);
void putcolor(void);
void putlogo(void);
void removelogo(void);
void putbackground(void);
void startup(void);
void shutdown(void);
void setupforms(void);
void displayform(void);
void removeform(void);
int defform(char form_string[]);
int showform(int num, int label);
int eraseform(int num, int label);
int showdata(int fnum, int field, char data[]);
int getdata(int fnum, char data[]);
int cleanform(int fnum, int field, char data[]);
void restore_screen(FILE *fptr, unsigned size);