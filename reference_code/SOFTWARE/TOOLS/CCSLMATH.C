/* CCSLMATH.C: Math functions

	Created by:		Scott Dillabough
	Date created:	October 21, 1993

*/

#include "ccslmath.h"

float convertit(float value, int conversion, char type)
{
/* This function will receive a number and convert it to
   its equivalent number in the opposite unit of measure.
   Parameters: VALUE      - the number to be converted
	       CONVERSION - the table (defined in this function)
			    number of the desired conversion formula.
			  - must be a number between 0 and 30, inclusive.
	       TYPE       - an indicator telling the function which
			    measurement system the result will be of.
			  - "I" or "i" for IMPERIAL
			  - "M" or "m" for METRIC
			  - function assumes METRIC for any other value
*/



    float result, conversions[31] =
	{ 2.54,0.305,0.914,1.609,5.029,
	  0.405,6.452,0.093,0.836,2.59,
	  28.35,0.454,0.907,
	  28.41,0.568,1.137,4.546,3.785,16.387,0.765,
	  0.746,2.45,
	  70.21,70.06,1.121,6.895,11.23,1.4,2.81,
	  1.609,
	  1 };
/*	{ 2.54,0.305,0.914,1.609,5.029,                LENGTH      */
/*	  0.405,6.452,0.093,0.836,2.59,                AREA        */
/*	  28.35,0.454,0.907,                           WIEGHT      */
/*	  28.41,0.568,1.137,4.546,3.785,16.387,0.765,  VOLUME      */
/*	  0.746,2.45,                                  POWER       */
/*	  70.21,70.06,1.121,6.895,11.23,1.4,2.81,      PROPORTION  */
/*	  1.609,                                       SPEED       */
/*	  1 }                                          TEMPERATURE */


    if (conversion < 30) /* any conversion except temperature */
    {
	if (type != 'i' && type != 'I') /* converting to METRIC */
	    result = value * conversions[conversion];
	else /* converting to IMPERIAL */
	    result = value / conversions[conversion];
    }
    else /* temperature conversion */
    {
	if (type != 'i' && type != 'I') /* converting to METRIC (CELCIUS) */
	    result = 5 / 9 * (value - 32);
	else /* converting to IMPERIAL (FERENHEIT) */
	    result = value * 9 / 5 + 32;
    }
    return(result);
}