/* DATABASE.C:  Database file management toolbox

	Created by:		Kevin Rattai
	Date created:	October 9, 1993

*/

#include<process.h>
#include<string.h>
#include"pxengine.h"
#include"database.h"

PXCODE pxErr;

int storecurrent(char *fromname, char *toname)
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

int readnext(char *fromname, char *toname)
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

int readprevious(char *fromname, char *toname)
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

int setuptable(char filename[], char *names[], char *types[])
{
	int exist;
    int nfields;


    nfields = sizeof(names)/sizeof(char *);

	if((pxErr = PXTblExist(filename, &exist)) == PXSUCCESS)
	{
    	if(!exist)
        {
        	if((pxErr = PXTblCreate(filename, nfields, names, types))
															!= PXSUCCESS)
			{
				putch(7);
			}
        }
    }
    else
    {
    	putch(7);
    }
    return(pxErr);
}

int loadvariables(char *name[], char *list[])
{
	*name=*list;
    return(0);
}