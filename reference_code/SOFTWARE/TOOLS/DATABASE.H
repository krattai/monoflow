/* DISK.H:  Disk file management tools header file

	Created by:		Kevin Rattai
	Date created:	July 5, 1993

*/

int storecurrent(char *filename, char *record);
int readnext(char *filename, char *record);
int readprevious(char *filename, char *record);
int setuptable(char filename[], char *names[], char *types[]);
int loadvariables(char *name[], char *list[]);