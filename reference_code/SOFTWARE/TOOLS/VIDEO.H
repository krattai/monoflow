/* VIDEO.H: define constants for Video_IO module

	Created by:		Kevin Rattai
	Date created:	June 1, 1992

*/

/* screen attributes */
#define NON_DISPLAY		0x00
#define UNDERLINE		0x01
#define NORMAL_DISPLAY  0x07
#define REVERSE_VIDEO	0x70
/* combine the following with the above values */
#define HI_INTENSITY	0x08
#define BLINK_BIT		0x80

/* video modes (as defined by the ROM BIOS) */
#define BW40_MODE		0
#define CO40_MODE		1
#define BW80_MODE		2
#define CO80_MODE		3
#define CO320_MODE		4
#define BW320_MODE		5
#define BW640_MODE		6
#define MONO_MODE		7

/* ROM BIOS function values */
/* (put one of these values into sreg.ah before calling */
/* the VIDEO_IO services int the ROM BIOS) */
#define V_INIT			0
#define V_CTYPE			1
#define V_SET_CUR		2
#define V_GET_CUR		3
#define V_SETPAGE		5
#define V_SCRLUP		6
#define V_SCRLDOWN		7
#define V_GCA			8
#define V_WCA			9
#define V_WC			10
#define V_WTTY			14
#define V_STATE			15

/* BIOS Video software interrupt */
#define V_INT_NO		0x10

/* function prototypes */
void vid_init(int);
int vid_state(int);
void vid_page(int);
void vid_get_cur(int *, int *);
void vid_set_cur(int, int);
void vid_clr_scn(int, int, int, int, int);
void vid_up(int, int, int, int, int, int);
void vid_down(int, int, int, int, int, int);
void vid_blank(int, int);
void vid_wca(int c, int a);
int vid_gca(char *pc, char *pa);
void vid_tc(int c);
int vid_info(int *);