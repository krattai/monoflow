/*
 ============================================================================
 Name        : fbp001.c
 Author      : Kevin Rattai
 Version     : 0.0.001
 Copyright   : working license: The BSD 2-Clause License -> http://opensource.org/licenses/bsd-license.php
 Description : main program for fbp001 in C, Ansi-style
 ============================================================================
*/

/* see comments at bottom of file */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	char fld1[10];
	char fld2[10];
	char fld3[10];
} TESTER;
TESTER testform = {"test","test2Yup","test3"};

/* inline function prototypes */
int startup(int);
int shutdown(void);

int main(int argc, char *argv[]) {
    int ctr;

    puts("Greetings, user.");

    puts("Checking...  please wait...");

    if(startup(argc)) {
    	for( ctr=1; ctr < argc; ctr++ ) {
    		puts( argv[ctr] );
    	}
    }

    shutdown();

    puts("Goodbye, user.");

    return EXIT_SUCCESS;

}

int startup(int check)
{
	/*	startup protocols */

	int yesno = 1;

    if (check < 2) {
    	puts("No arguments.  New environment.");

    	yesno = 0;
    }


	return(yesno);
}

int shutdown(void)
{
	/*	shutdown protocols */

	int buts = 1;

   	puts("Cleaning up for shutdown.");

	return(buts);
}

/*
 * So main() is basically an on/off switch.
 * Main calls the primary node or entity, which in turn runs
 * the primary node may or may not take data.  Main
 * would need to know that.
 *
 * An entity / node consists of a label, data, instructions,
 * and trigger.  In a natural world, the input / trigger
 * point may not be obvious, but for the initial version
 * we will assume that we know the input point if required
 * or we assume that it is just a trigger.
 *
 * No, I think main will take input which will be an array
 * which would then be sent to the primary node.
 *
 * Anyhow, once the primary node fires, it starts a chain
 * reaction of processing its input and triggering other
 * nodes as necessary.
 */

/* here's A mid term goal (something I've prototyped in the
 *  past):
 *
 *  undefined_function(&instructions, &data);
 *
 *  What does that mean?
 *
 *  Well, it means that I can have an application that looks
 *  like this (I'm going to use pseudocode):
 *
 *  int main(&fbp_mandate) {
 *
 *     fopen(fbp_mandate);
 *
 *     while fbp_mandate {
 *
 *        undefined_function(*fbp_mandate[a], *fbp_mandate[b]);
 *        ++fbp_mandate;
 *
 *     }
 *
 *     fclose(fbp_mandate);
 *     return;
 *
 *  }
 *
 *  That's it.  An application of variable size and scope, with
 *  a completely arbitrary instruction and data set, that runs
 *  in a linear fashion.
 *
 *  The internal workings of undefined_function() load the
 *  instructions (precompiled) and data (predefined) references
 *  from the file fbp_mandate, which references other files or
 *  predefinitions of instructions and data.
 *
 *  I don't load more of the application or data than I need at
 *  any given time.
 *
 */
