/* ASMTOOLS.H: Low Level Byte Manipulation module

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/

#include"cminor.h"

/* include header file to declare */
/* port I/O functions */
#include<dos.h>

typedef union						/* 32 bit conversion data type */
{
	long l;							/* viewed as long value */
	struct							/* viewed as two unsigned values */
	{
		unsigned low_word;
		unsigned high_word;
	} words;
} long_or_words;

typedef union						/* 16 bit conversion data type */
{
	int i;							/* viewed as integer */
	struct							/* viewed as two bytes */
	{
		unsigned char low_byte;
		unsigned char high_byte;
	} bytes;
} int_or_bytes;

/* flag bit definitions */
#define ZF_FLAG 0x40
#define AF_FLAG 0x10
#define PF_FLAG 0x04
#define CF_FLAG 0x01
#define TF_FLAG 0x0100
#define SF_FLAG 0x0080
#define IF_FLAG 0x0200
#define DF_FLAG 0x0400
#define OF_FLAG 0x0800

/* memory access functions */
void lmove(word16, word16, word16, word16, int);
unsigned getbyte(unsigned, unsigned);
void putbyte(int, unsigned, unsigned);

/* port I/O macros */
#define inbyte(p)		(inport(p))
#define outbyte(p,b)	(outportb(p,b))

/* macros to convert between far pointers and 8086 */
/* segment:address values */
#define SET_FP_OFF(p,o)	((* (unsigned *) & p) = o)
#define SET_FP_SEG(p,s) ((* (((unsigned *) & p) + 1)) = s)
#define SET_FP(p,s,o)	(SET_FP_SEG(p,s), SET_FP_OFF(p,o))
#define GET_FP_OFF(p)	((* (unsigned *) & p))
#define GET_FP_SEG(p)	((* (((unsigned *) & p) + 1)))

/* functions to convert pointers to segment and offset values */
void get_dads(void *, word16 *, word16 *);
void get_fads(void (far *) (), word16 *, word16 *);