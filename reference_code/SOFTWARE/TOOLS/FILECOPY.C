/* FILECOPY.C:  Disk file management test program

	Created by:		Kevin Rattai
	Date created:	July 5, 1993

*/

#include<process.h>
#include<string.h>
#include"disk.h"

void main(void)
{
	fcopy("*.*","B:\*.*");
}