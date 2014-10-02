/* BIOSAREA.H: ROM BIOS data area at 0x0040:0 in memory

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/
										 /* user created header */
#ifndef  byte
#define  byte   unsigned char		   /* define byte as a type */
#endif

/* BIT FIELDS USED IN ROM BIOS DATA AREA */
typedef struct {
   unsigned	hasFloppies : 1,	/* 1 = system has floppy drives */
			nu1 : 1,								/* not used */
			mbRAM : 2,		 /* motherboard RAM size (obsolete) */
			initVideo : 2,				  /* initial video mode */
			nDisks : 2,					/* nbr of floppy drives */
			nu8 : 1,								/* not used */
			nSerialPorts : 3,	/* nbr of serial ports attached */
			gamePort : 1,			  /* 1 = game port attached */
			nu13 : 1,								/* not used */
			nLPT : 2;					  /* number of printers */
} EQFLAGS;			   /* this is the equipment flags structure */

typedef struct {
   unsigned	riteShiftDown : 1,		/* 1 = right shift key down */
			leftShiftDown : 1,		 /* 1 = left shift key down */
			ctrlShiftDown : 1,	   /* 1 = ctrl-shift combo down */
			altShiftDown : 1,		/* 1 = alt-shift combo down */
			scrollLockOn : 1,		 /* 1 = scroll lock mode on */
			numLockOn : 1,				/* 1 = num lock mode on */
			capsLockOn : 1,			   /* 1 = caps lock mode on */
			insOn : 1,						 /* 1 = ins mode on */
			unused : 3,							  /* spare bits */
			ctrlNumLockOn : 1,		/* 1 = ctrl-NumLock mode on */
			scrollLockDown : 1,		/* 1 = scroll lock key down */
			numLockDown : 1,		   /* 1 = num lock key down */
			capsLockDown : 1,		  /* 1 = caps lock key down */
			insDown : 1;					/* 1 = ins key down */
} KBDFLAGS;				/* this is the keyboard flags structure */

typedef struct {
	unsigned	serialPortAddr[4];
	unsigned	parallelPortAddr[4];
	EQFLAGS 	eqptFlags;
	byte		mfgrTestFlags;
	unsigned	mainMem;
	unsigned	expRAM;
	KBDFLAGS	kbdStat;
	byte		keypad;
	unsigned	kbdBuffHead;
	unsigned	kbdBuffTail;
	char		kbdBuff[32];
	byte		seekStat;
	byte		motorStat;
	byte		motorCnt;
	byte		diskErr;
	byte		NECStatus[7];
	byte		videoMode;
	unsigned	scrnWidth;
	unsigned	vidBuffSz;
	unsigned	vidBuffOfs;
	byte		cursPos[8][2];
	byte		cursBottom;
	byte		cursTop;
	byte		activeDispPage;
	unsigned	activeDispPort;
	byte		CRTModeReg;
	byte		palette;
	unsigned	dataEdgeTimeCount;
	unsigned	CRCReg;
	char		lastInputValue;
	unsigned	tick;
	int			hour;
	byte		timeOverflow;
	byte		brkStat;
	unsigned	resetFlag;
	long		hardDiskStat;
	byte		parallelTimeout[4];
	byte		serialTimeout[4];
	unsigned	kbdBuffOfs;
	unsigned	kbdBuffEnd;
} BIOSDATA;