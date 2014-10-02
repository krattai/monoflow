/* MEMORY.C: access to PC RAM memory

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/

#include<dos.h>
#include"asmtools.h"

unsigned getbyte(seg,off)			/* get a byte from a memory address */
unsigned seg;						/* segment address */
unsigned off;						/* offset address */
{
	unsigned char far *pmem;

	SET_FP(pmem,seg,off);

	return(*pmem);
}

void putbyte(value,seg,off)			/* store a byte at a memory address */
int value;							/* value to store */
unsigned seg;						/* segment address */
unsigned off;						/* offset address */
{
	unsigned char far *pmem;

	SET_FP(pmem,seg,off);
	*pmem = value;
}

									/* move bytes from one memory */
									/* address to another */
void lmove(to_seg,to_off,from_seg,from_off,nbytes)
word16 to_seg;						/* destination segment address */
word16 to_off;						/* destination offset address */
word16 from_seg;					/* source segment address */
word16 from_off;					/* source offset address */
int nbytes;
{
	unsigned char far *pto;
	unsigned char far *pfrom;

	SET_FP(pto,to_seg,to_off);
	SET_FP(pfrom,from_seg,from_off);

	while(nbytes > 0)
	{
		*pto = *pfrom;
		pto++;
		pfrom++;
		nbytes--;
	}
}