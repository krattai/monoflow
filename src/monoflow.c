/*
 ============================================================================
 Name        : monoflow.c
 Author      : Kevin Rattai
 Version     : 0.0.001
 Copyright   : working license: The BSD 2-Clause License -> http://opensource.org/licenses/bsd-license.php
 Description : main program for monoflow001 in C, Ansi-style
 ============================================================================
*/

/* really need to work on being able to construct a node */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"scheduler.h"

/* inline function prototypes */
int startup(int);
int shutdown(void);

/* pass on ptr arry, must provide full decl */
int explode(int cnt, char *flat[]);

int main(int argc, char *argv[]) {

    if(startup(argc)) {

    	/* pass on ptr arry sent as follows */
    	explode(argc, argv);

    }

    shutdown();

    return EXIT_SUCCESS;
}


/* as defined, exactly as main() is */
int explode(int cnt, char *flat[]) {
	int ctr = 0;
	int swt = 0;
	int temp = 0;
	int t2 = 0;

	/* as from cli input, first entry is exe prgm */
	for( ctr=1; ctr < cnt; ctr++ ) {

		/* check if not MF instruction */
		if(strcmp(flat[ctr], "MF$") == 0) {

			ctr++;
			while(!strcmp(flat[ctr], "MF!") == 0) {

				/* this adds the value of the characters
				 * in the ctr position of flat[]
				 */
				for(t2=0; t2 < strlen(flat[ctr]); t2++) {
					swt += *(flat[ctr] + t2);
				}

				switch(swt) {
					case 217 :
						ctr++;
						printf("%d\n", atoi(flat[ctr]) );
						temp = atoi(flat[ctr]);
						break;
					case 201 :
						ctr++;
						printf("%d -> ", atoi(flat[ctr]) );
						temp += atoi(flat[ctr]);
						printf("%d\n", temp );
						break;
					case 234 :
						ctr++;
						printf("%d -> ", atoi(flat[ctr]) );
						temp -= atoi(flat[ctr]);
						printf("%d\n", temp );
						break;
					case 238 :
						ctr++;
						printf("%d -> ", atoi(flat[ctr]) );
						temp = temp*atoi(flat[ctr]);
						printf("%d\n", temp );
						break;
					case 227 :
						ctr++;
						printf("%d -> ", atoi(flat[ctr]) );
						temp = temp/atoi(flat[ctr]);
						printf("%d\n", temp );
						break;
					default :
						printf("Invalid instruction %s\n", flat[ctr] );
						ctr++;
						break;
				}

				swt = 0;

				ctr++;
			}
		}
		else {
		    printf("\n%s\n", flat[ctr]);
		}
	}

	return ctr;
}

int startup(int check)
{
	/*	startup protocols */

	int yesno = 1;

    if (check < 2) {
        printf("No arguments.  New environment.\n\n");

    	yesno = 0;
    }

	return(yesno);
}

int shutdown(void)
{
	/*	shutdown protocols */

	int yesno = 1;

	return(yesno);
}
