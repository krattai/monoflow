/*
 ============================================================================
 Name        : scheduler.c
 Author      : Kevin Rattai
 Version     : 0.0.001
 Copyright   : working license: The BSD 2-Clause License -> http://opensource.org/licenses/bsd-license.php
 Description : scheduler header for monoflow in C, Ansi-style
 ============================================================================
*/

/* This is just a header template for what traditional fbp requires */
/*  scheduling would be a buffering system of passing messages or IPs */
/*  between nodes */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"scheduler.h"

/* constants */

/* structures */
typedef struct {
	int var1;
	int var2;
	int var3;
} schd_info;

/* non-functional inline function prototype templates */
int startup(int);
int shutdown(void);

/* function prototypes */
void schd_prim(void);
