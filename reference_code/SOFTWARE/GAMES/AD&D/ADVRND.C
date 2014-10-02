/* ADVRND.C:  Tests random number generation based on time funtion

	Created by:		Kevin Rattai
	Date created:	May 1, 1993

*/
#include<stdlib.h>
#include<time.h>

int rnd(int);

/* prints a random number in the range 0-99 */
main()
{
	int n, x, y,z;

	clrscr();
	n = 100;
	randomize();
	printf("\start\n");
	y = rnd(5);
	printf("\ny = %d\n",y);
	getch();
	while(y-->0)
	{
		z = rnd(20);
		while(z-->0)
		{
			x=rnd(n);
			printf("%d ",x);
		}
		printf("\n");
		z = rnd(20);
		while(z-->0)
		{
			x=rnd(n);
			printf("%d ",x);
		}
		printf("\n\n");
	}
}

int rnd(int number)
{
	/* selects a random number between 1 and 20 */
	number++;
	srand((unsigned) random(1000));
	return(random(number));
}