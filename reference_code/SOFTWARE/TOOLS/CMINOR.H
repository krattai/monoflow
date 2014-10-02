/* CMINOR.H: enhancements to C syntax

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/

/* use the following when the compiler treats char as signed */
#define tochar(c)	((c) & 0xff)

/* use the following when char is treated as unsigned */
/* #define tochar(c)	c */

/* toascii - strip parity bit from a char value */
#define toascii(c)	((c) & 0x7f)

/* test a char to see whether the high order bit is zero */
#define isascii(c)	(((c) & 0x80) == 0)

/* test a char to see if it is printable (ASCII graphics) */
#define isgraphic(c)	(((c) >= ' ') && ((c) <= '~'))

/* fixed size portable data types */
/* define with both upper and lower case */
typedef int				int16;	/* signed 16 bit integer */
typedef int				INT16;	/* signed 16 bit integer */
typedef long			int32;	/* signed 32 bit integer */
typedef long			INT32;	/* signed 32 bit integer */
typedef unsigned		word16;	/* unsigned 16 bit number */
typedef unsigned		WORD16;	/* unsigned 16 bit number */
typedef unsigned long	word32;	/* unsigned 32 bit number */
typedef unsigned long	WORD32;	/* unsigned 32 bit number */