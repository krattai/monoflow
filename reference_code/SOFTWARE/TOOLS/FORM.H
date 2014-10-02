/* FORM.H: header file for form manipulation

	Created by:		Kevin Rattai
	Date created:	June 1, 1993

*/
#include<conio.h>

/* Condition Definitions */
#define EQUAL		=
#define MORE		>
#define LESS		<
#define NOT			!
#define AND			&
#define OR			|
#define CLS			clrscr();
#define SPLIT		0x02

/* Function Prototypes */
int	Show_form(char form_string[]);
int	Put_form_info(char form_string[], char form_info[]);
int	Get_form_info(char form_string[], char* form_info);
int Get_form_string(char *, int);
int Put_window(int, int, int, int, int, int);
int	Wait_form(char form_string, int conditions[]);
int Put_original(struct text_info);
int Parse_coded_info(char *);
int Un_parse_info(char *);
int Load_dict(void);