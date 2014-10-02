/* TTY1.C: bare-bones tty emulation program

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/

#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include"cminor.h"
#include"keyio.h"
#include"async.h"
#include"video.h"


#define CARD		COM1				/* which RS-232 port to use */
#define THRU_KEY	F1KEY				/* define key for exiting pgm */

main(void)
{
	int c;
	int ret;

	ret = comm_init(CARD);				/* set up for RS-232 use */
	if(ret != 0)						/* check for error */
	{
		printf("unrecognizable RS232 address - %x \n",ret);
		exit(10);
	}
	c = ' ';							/* force execution the first time */
	while(c != THRU_KEY)
	{
		if(chk_rcv() != 0)				/* check for rcvd data */
		{
			c = rcv_char();				/* yes - get it */
			vid_tc(c);					/*		 and display it */
		}
		else
			if(keypress() != 0)			/* check for keybd input */
			{
				c = getkey();			/* yes - get it */
				putchar();
				if(c < 0x80)			/*		 and transmit if ASCII */
					send_char(c);
			}
	}
}