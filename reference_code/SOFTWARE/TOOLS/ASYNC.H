/* ASYNC.H: defines constants for async I/O

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/

/* async port names */
#define COM1	0
#define COM2	1

/* software interrupt number for ROM BIOS async support */
#define RS232_IO	0x14

/* service codes for rom BIOS calls (put in AX) */
#define RS_INIT		0x0000
#define RS_XMT		0x0100
#define RS_RCV		0x0200
#define RS_STATUS	0x0300

/* bit definitions for BIOS status returns */
#define TIM_OUT_BIT 0x8000
#define XMT_RDY_BIT 0x2000
#define BRK_DET_BIT 0x1000
#define FRM_ERR_BIT 0x0800
#define PAR_ERR_BIT 0x0400
#define OVR_RUN_BIT 0x0200
#define DTA_RDY_BIT 0x0100
#define CAR_DET_BIT 0x0080
#define RNG_IND_BIT 0x0040
#define DSR_BIT		0x0020
#define CTS_BIT		0x0010

/* mask for receive error conditions */
/* (TIM_OUT_BIT | FRM_ERR_BIT | PAR_ERR_BIT | OVR_RUN_BIT) */
#define RCV_ERR_BIT 0x8E00

/* the following are parms for the RS232_init service */
/* choose one from each group and combine (+ or |) */
/* and put into REGS.AX (with service code) */

/* baud rates */
#define BAUD_110	0x00
#define BAUD_300	0x40
#define BAUD_1200	0x80
#define BAUD_4800	0xc0

/* parity settings */
#define PAR_NONE	0x00
#define PAR_ODD		0x08
#define PAR_EVEN	0x18

/* number of stopbit settings */
#define STOP_1		0x00
#define STOP_2		0x04

/* data word length settings */
#define DATA_7		0x02
#define DATA_8		0x03

/* I/O port offsets for async card */
/* (add the base address for the card being used) */
#define RCV_DATA		0
#define XMT_DATA		0
#define INT_ENABLE		1
#define INT_ID			2
#define LINE_CTRL		3
#define MODEM_CTRL		4
#define LINE_STATUS		5
#define MODEM_STATUS	6

/* Line Status Register Bits */
#define XMT_RDY		0x20		/* 1 = ok to transmit */
#define BRK_DET		0x10		/* 1 = break received */
#define FRM_ERR		0x08		/* 1 = framing error detected */
#define PAR_ERR		0x04		/* 1 = parity error detected */
#define OVR_RUN		0x02		/* 1 = rcv overrun */
#define DTA_RDY		0x01		/* 1 = rcvd data ready */
#define RCV_ERRS	0x1E		/* all rcv error bits above */

/* Modem Status Register Bits */
#define CAR_DET		0x80
#define RNG_IND		0x40
#define DSR			0x20
#define CTS			0x10

/* various control register bits */
#define SET_BRK_BIT		0x40
#define INT_PENDING		0x01
#define ENABLE_RCV_INT	0x01
#define OUT2			0x08	/* in MCR = 1 to allow ints */

/* 1st I/O port addresses for com1: and com2: */
#define PRIMARY			0x3f8
#define SECONDARY		0x2f8

/* corresponding interrupt numbers */
#define PRI_INT			4
#define SEC_INT			3

/* function prototypes */
int comm_init(int);
int chk_rcv(void);
int rcv_char(void);
int chk_xmt(void);
void xmt_char(int);
void send_char(int);