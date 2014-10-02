/* KEYIO.H: definitions of values returned by getkey()

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/

/* keypressed return values */
#define NO_INPUT	0
/* non-zero return means there is input waiting */

/* ASCII control characters */
#define ASCNUL		(256+3)
#define ASCBEL		7
#define BELL		7
#define ASCBS		8
#define BACKSPACE	8
#define ASCTAB		9
#define TAB			9
#define ENTER		13
#define ASCLF		0xA
#define ASCFF		0xC
#define ASCCR		0xD
#define ASCESC		0x1B
#define ESC			0x1B
#define ASCDEL		0x7F
#define DEL			0x7F
#define ASCSPACE	0x20

/* special keys for IBM PC */
#define HOMEKEY		(256+71)				/*  -71 */
#define HOME		-71
#define BACKTAB		(256+15)				/*  -15 */
#define BKTAB		-15
#define UPARROW		(256+72)				/*  -72 */
#define UP			-72
#define LEFTARROW	(256+75)				/*  -75 */
#define LEFT		-75
#define RIGHTARROW	(256+77)				/*  -77 */
#define RIGHT		-77
#define ENDKEY		(256+79)				/*  -79 */
#define END			-79
#define DOWNARROW	(256+80)				/*  -80 */
#define DOWN		-80
#define PGUPKEY		(256+73)				/*  -73 */
#define PGUP		-73
#define PGDNKEY		(256+81)				/*  -81 */
#define PGDN		-81
#define INSERTKEY	(256+82)				/*  -82 */
#define INSERT		-82
#define DELETEKEY	(256+83)				/*  -83 */
#define DELETE		-83
#define CTLPRTSC	(256+114)				/* -114 */
#define CTRLPRT		-114
#define CTLLARROW	(256+115)				/* -115 */
#define CTRLLEFT	-115
#define CTLRARROW	(256+116)				/* -116 */
#define CTRLRIGHT	-116
#define CTLENDKEY	(256+117)				/* -117 */
#define CTRLEND		-117
#define CTLPGDNKEY	(256+118)				/* -118 */
#define CTRLPGDN	-118
#define CTLHOMEKEY	(256+119)				/* -119 */
#define CTRLHOME	-119
#define CTLPGUPKEY	(256+132)				/* -132 */
#define CTRLPGUP	-132

/* function key codes */
#define F1KEY		(256+59)				/*  -59 */
#define F1			-59
#define F2KEY		(256+60)				/*  -60 */
#define F2			-60
#define F3KEY		(256+61)				/*  -61 */
#define	F3			-61
#define F4KEY		(256+62)				/*  -62 */
#define F4			-62
#define F5KEY		(256+63)				/*  -63 */
#define F5			-63
#define F6KEY		(256+64)				/*  -64 */
#define F6			-64
#define F7KEY		(256+65)				/*  -65 */
#define F7			-65
#define F8KEY		(256+66)				/*  -66 */
#define F8			-66
#define F9KEY		(256+67)				/*  -67 */
#define F9			-67
#define F10KEY		(256+68)				/*  -68 */
#define F10			-68

/* shift-function key codes */
#define SHFTF1KEY		(256+84)				/*  -84 */
#define SHFTF2KEY		(256+85)
#define SHFTF3KEY		(256+86)
#define SHFTF4KEY		(256+87)
#define SHFTF5KEY		(256+88)
#define SHFTF6KEY		(256+89)
#define SHFTF7KEY		(256+90)
#define SHFTF8KEY		(256+91)
#define SHFTF9KEY		(256+92)
#define SHFTF10KEY		(256+93)

/* ctrl-function key codes */
#define CTRLF1KEY		(256+94)				/*  -94 */

/* alt-function key codes */
#define ALTF1KEY		(256+104)				/* -104 */

/* alt-letter key codes */
#define ALTR		-19
#define ALTY		-21
#define ALTI		-23
#define ALTP		-25
#define ALTS		-31
#define ALTJ		-36
#define ALTX		-45

/* alt-key + number key (top row) */
#define ALT1KEY		(256+120)				/* -120 */
#define ALT2KEY		(256+121)
#define ALT3KEY		(256+122)
#define ALT4KEY		(256+123)
#define ALT5KEY		(256+124)
#define ALT6KEY		(256+125)
#define ALT7KEY		(256+126)
#define ALT8KEY		(256+127)
#define ALT9KEY		(256+128)
#define ALT0KEY		(256+129)

/* function prototypes */
int getkey(void);
int Get_key(void);
int get_key(void);
int keypress(void);
void keyflush(void);
void waitcr(void);
