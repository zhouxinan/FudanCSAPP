/**************************************************************************
  Row/column summation functions.  Edit this file as follows:
   1. Put your name and Andrew ID in the comment below
   2. Implement different versions of the summation functions
   3. Edit the table at the end to include your implementations.  Put
      your best solutions (best row & column sum, best column sum)
      as the first two entries in this table.
***************************************************************************/
   

/**************************************************************************
 Edit this comment to indicate your name and Andrew ID
   Submission by Harry Q. Bovik, bovik@andrew.cmu.edu
***************************************************************************/





#include  <stdio.h>
#include  <stdlib.h>
#include  "rowcol.h"
#include  <math.h>

/* Reference implementation of column sum. */
/* Compute sum of each column in matrix.  Note that the argument
   conventions are the same for row & column sum, but 2nd argument
   is never referenced
*/

void c_sum(matrix_t M, vector_t rowsum, vector_t colsum)
{
    int i,j;
    for (j = 0; j < N; j++) {
	colsum[j] = 0;
	for (i = 0; i < N; i++)
	    colsum[j] += M[i][j];
    }
}


/* Reference implementation of row & column sum */
/* Compute sum of each row & of each column in matrix */

void rc_sum(matrix_t M, vector_t rowsum, vector_t colsum)
{
    int i,j;
    for (i = 0; i < N; i++) {
	rowsum[i] = colsum[i] = 0;
	for (j = 0; j < N; j++) {
	    rowsum[i] += M[i][j];
	    colsum[i] += M[j][i];
	}
    }
}



/* This table contains a list, where each entry gives the name
   of an summation function,
   the value ROWCOL if the function sums both rows and columns or
   COL if it only sums columns.  It also has a a string describing
   the function.
   Use this table to create a list with a number of different solutions.
   Then pick the best ones for your actual solution
*/

rc_fun_rec rc_fun_tab[] = 
{

  /* Your first entry should be the best column summation function */
    {c_sum, COL, "Best column sum"},
  /* Your second entry should be the best row & column summation function */
    {rc_sum, ROWCOL, "Best row and column sum"},

    {c_sum, COL, "Column sum, reference implementation"},

    {rc_sum, ROWCOL, "Row and column sum, reference implementation"},

 /* This is the null terminator that marks the end of the function list.
    Leave this here. */
    {NULL,ROWCOL,NULL}
};
