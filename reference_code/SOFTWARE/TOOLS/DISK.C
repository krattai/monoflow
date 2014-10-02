/* DISK.C:  Disk file management toolbox

	Created by:		Kevin Rattai
	Date created:	July 5, 1993

*/

#include<process.h>
#include<string.h>
#include"disk.h"

int fcopy(char *fromname, char *toname)
{
	char temp[80];

	strcpy(temp,"copy ");
	strncat(temp,fromname,strlen(fromname));
	strncat(temp,",",strlen(","));
	strncat(temp,toname,strlen(toname));
	strncat(temp," > NULL",strlen(" > NULL"));
	system(temp);
	system("del NULL.");

	return(0);
}