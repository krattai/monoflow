
/* ASYNC1.C: basic async I/O module

	Created by:		Kevin Rattai
	Dated created:	June 1, 1992

*/

#include<stdio.h>
#include"asmtools.h"
#include"async.h"

typedef struct
{
	int base_port;					/* 1st I/O port for this async card */
	int int_no;						/* associated interrupt level */
} ASY_ADRS;

#define BIOS_DATA	0x40			/* segment address of BIOS data area */

int rs232card;						/* save card number here */
int iobase;							/* 1st port address of async card */
int iasync;							/* index into aadr table */

ASY_ADRS aadr[2] =
{
	PRIMARY, PRI_INT,				/* primary async adapter (COM1:) */
	SECONDARY, SEC_INT				/* secondary adapter (COM2:) */
};

int comm_init(card)
int card;							/* card number for RS232 I/O */
{
	rs232card = card;				/* save card number */
									/* get the corresponding port */
									/* address from the BIOS data seg */
	iobase = getbyte(BIOS_DATA,card*2) |
		(getbyte(BIOS_DATA,card*2+1) << 8);
	switch(iobase)					/* check for valid I/O address */
	{
		case PRIMARY :		iasync = 0;	break;
		case SECONDARY :	iasync = 1; break;
		default :			return(iobase); /* not a valid I/O address */
							break;
	}
	return(0);
}

int chk_rcv()						/* check for received char */
{
	int s;

	s = inbyte(iobase + LINE_STATUS); /* get line status */
	return(s & DTA_RDY);
}

int rcv_char()						/* get a received char */
{
	int s, c;
									/* check status for errors */
	s = inbyte(iobase + LINE_STATUS) & RCV_ERRS;
	c = inbyte(iobase + RCV_DATA);	/* get the char itself */
	if(s == 0)
		return(tochar(c));			/* no errors, return char */
	else
		return(-1);					/* error - return (-1) */
}

int chk_xmt()						/* check to see if ready */
{									/* to transmit next char */
	int s;

	s = inbyte(iobase + LINE_STATUS); /* get the line status */
	return(s & XMT_RDY);
}

void xmt_char(c)					/* transmit a char */
int c;								/* char to be transmitted */
{
	outbyte(iobase + XMT_DATA,c);
}

void send_char(c)					/* check Xmt status and then */
int c;								/* transmit this char */
{
	while(chk_xmt() == 0);			/* wait until ok to xmt char */
	{
		;
	}
	xmt_char(c);					/* transmit the char */
}