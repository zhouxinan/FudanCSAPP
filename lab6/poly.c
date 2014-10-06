/**************************************************************************
  Polynomial evaluation functions.  Edit this file as follows:
   1. Put your name and Andrew ID in the comment below
   2. Implement different versions of the polynomial evaluation function
   3. Edit the table at the end to include your implementations.  Put
      your best solutions (minimum CPE, minimum C10) as the first
      two entries in this table.
***************************************************************************/
   




#include  <stdio.h>
#include  <stdlib.h>
#include  "poly.h"


/**************************************************************************
 Edit this comment to indicate your name and Andrew ID
   Submission by Harry Q. Bovik, bovik@andrew.cmu.edu
***************************************************************************/


/* Polynomial evaluation.  Reference implementation */

int poly_eval(int *a, int degree, int x)
{
    int result = 0;
    int i;
    int xpwr = 1; /* Successive powers of x */
    for (i = 0; i <= degree; i++) {
	result += a[i]*xpwr;
	xpwr   *= x;
    }
    return result;
}



/* This table contains a list, where each entry gives the name
   of an evaluation function, along with a string describing
   the function.
   Use this table to create a list with a number of different solutions.
   Then pick the best ones for your actual solution
*/
   
peval_fun_rec peval_fun_tab[] = 
{

  /* Your first entry should be the function with best CPE */
 {poly_eval, "Best CPE"},
  /* Your second entry should be the function with best
     performance on degree 10 */
 {poly_eval, "Best for 10"},

 {poly_eval, "poly_eval: Reference implementation"},

 /* This is the null terminator that marks the end of the function list.
    Leave this here. */
 {NULL, ""}
};







