/*
 ============================================================================
 Name        : network.c
 Author      : Kevin Rattai
 Version     : 0.0.001
 Copyright   : working license: The BSD 2-Clause License -> http://opensource.org/licenses/bsd-license.php
 Description : network header for monoflow in C, Ansi-style
 ============================================================================
*/

/* This is just a template for what traditional fbp requires */
/*  network is the management of passing messages or IPs between nodes */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* constants */
# define CONSTANT 1

/* structures */
typedef struct {
	int var1;
	int var2;
	int var3;
} nwrk_info;

/* non-functional inline function prototype templates */
int startup(int);
int shutdown(void);

/* function prototypes */
void nwrk_prim(void);
