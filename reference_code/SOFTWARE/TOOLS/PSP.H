/* PSP.H: Defines the program segment prefix structure

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/

typedef struct {
	unsigned int20h,					/* pgm term call (no value) */
				topofmem;				/* segment of top of memory */
	char		res1,									/* reserved by DOS */
				int21h [5];					  /* no value in Turbo C */
	long		oldtermvec,		  /* old int 22h interrupt vector */
				oldcbvec,      	  /* old Ctrl-Break int vector */
				oldcritvec;		 /* old critical error int vector */
	char		res2 [22];							  /* reserved area */
	unsigned	envblock;		  /* segment of environment block */
	char		res3 [46],							  /* reserved area */
				avail [36],                /* FCB's: open memory */
				cmdtaillen,			     /* length of command tail */
				cmdtail [127];              /* command tail text */
} PSPSTRUCT;