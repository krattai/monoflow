#include<process.h>
/* #include<conio.h> */

void main(void)
{
	printf("\n\nEntering DOS environment\nType EXIT and press ENTER to"
			" return to the application\n");
	spawnle(P_WAIT,"envrmnt9","","");
}