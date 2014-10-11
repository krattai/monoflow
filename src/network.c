/*
 ============================================================================
 Name        : network.c
 Author      : Kevin Rattai
 Version     : 0.0.001
 Copyright   : working license: The BSD 2-Clause License -> http://opensource.org/licenses/bsd-license.php
 Description : network program for monoflow in C, Ansi-style
 ============================================================================
*/

/* This is just a template for what traditional fbp requires */
/*  network is the management of passing messages or IPs between nodes */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* non-functional inline function prototype templates */
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
