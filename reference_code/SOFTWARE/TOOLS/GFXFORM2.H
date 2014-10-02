/* GFXFORM2.H: Graphics form header based on original GRFXFORM.H

	Created by:		Kevin Rattai
	Date created:	June 9, 1993

*/

/* UVEA colors */
#define UVEABLACK		0
#define UVEALTBLUE		1
#define UVEADKBLUE		2
#define UVEAGREY		3
#define UVEAWHITE		4
#define UVEACHARCOAL	5

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
int defform(char form_string[]);
int showform(int num, int label);
int eraseform(int num, int label);
int showdata(int fnum, int field, char data[]);
int getdata(int fnum, char data[]);
int cleanform(int fnum, int field, char data[]);
void restore_screen(FILE *fptr, unsigned size);
int sprdsht(int fnum, int depth);
int removesprdsht(int fnum, int depth);