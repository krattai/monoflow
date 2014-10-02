/* CURSOR.C: Cursor apperance functions

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/

#include <dos.h>
#include "biosarea.h"

BIOSDATA far *bios = MK_FP (0x0040, 0);

void cursoff (void)
{                                /* turn hardware cursor off */
union REGS reg;

  reg.h.ah = 1;
  reg.h.ch = bios->cursTop | 0x20;          /* turn on bit 5 */
  reg.h.cl = bios->cursBottom;
  int86 (0x10, &reg, &reg);
}

void curson (void)
{                                 /* turn hardware cursor on */
union REGS reg;

  reg.h.ah = 1;
  reg.h.ch = bios->cursTop & 0xDF;         /* turn off bit 5 */
  reg.h.cl = bios->cursBottom;
  int86 (0x10, &reg, &reg);
}

void cursShape (int top, int bottom)
{                                     /* change cursor shape */
union REGS reg;

  reg.h.ch = top;
  reg.h.cl = bottom;
  reg.h.ah = 1;
  int86 (0x10, &reg, &reg);
}