/* Test setup for matrix row/col summing.  Do not change this. */
#include <stdio.h>
#include <stdlib.h>
#include "rowcol.h"
#include "fcyc.h"
#include "clock.h"

/* Define performance standards */
static struct {
  double cref;  /* Cycles taken by reference solution */
  double cbest; /* Cycles taken by our best implementation */
} cstandard[2] = 
{{21.0, 3.10}, /* Column Sum */
 {27.0, 5.50} /* Row & Column Sum */
};

/* Put in code to align matrix so that it starts on a cache block boundary.
   This makes the cache performance of the code a bit more predictable
*/

/* Words per cache block.  OK if this is an estimate as long as it
   is a multiple of the actual value
*/
#define WPB 16

int data[N*N+WPB];
int *mstart;

typedef vector_t *row_t;

/* Reference row and column sums */
vector_t rsref, csref, rcomp, ccomp;

static void init_tests()
{
    int i, j;
    int bytes_per_block = sizeof(int) * WPB;
    /* round mstart up to nearest block boundary */
    mstart = (int *)
      (((int) data + bytes_per_block-1) / bytes_per_block * bytes_per_block);
    for (i = 0; i < N; i++) {
	rsref[i] = csref[i] = 0;
    }
    for (i = 0; i < N; i++) {
	for (j = 0; j < N; j++) {
	    int val = random();
	    mstart[i*N+j] = val;
	    rsref[i] += val;
	    csref[j] += val;
	}
    }
}


/* Test function on all values */
int test_rc(rc_fun f, FILE *rpt, rc_comp_t rc_type) {
    int i;
    int ok = 1;

    for (i = 0; i < N; i++)
	rcomp[i] = ccomp[i] = 0xDEADBEEF;
    f((row_t)mstart, rcomp, ccomp);

    for (i = 0; ok && i < N; i++) {
	if (rc_type == ROWCOL
	    && rsref[i] != rcomp[i]) {
	    ok = 0;
	    if (rpt)
		fprintf(rpt,
			"Incorrect value for row %d.  Should be %d, got %d\n",
			i, rsref[i], rcomp[i]);
	}
	if ((rc_type == ROWCOL || rc_type == COL)
		 && csref[i] != ccomp[i]) {
	    ok = 0;
	    if (rpt)
		fprintf(rpt,
			"Incorrect value for col %d.  Should be %d, got %d\n",
			i, csref[i], ccomp[i]);
	}

    }
    return ok;
}

/* Kludgy way to interface to cycle measuring code */
void do_test(int *intf)
{
  rc_fun f = (rc_fun) intf;
  f((row_t)mstart, rcomp, ccomp);
}

void time_rc(rc_fun f, rc_comp_t rc_type, char *descr, double *cycp)
{
  int *intf = (int *) f;
  double t, cme;
  printf("Function: %s\n", descr);
  if (test_rc(f, stdout, rc_type)) {
    t = fcyc(do_test, intf);
    cme = t/(N*N);
    printf("  Total Cycles = %.2f, Avg Cycles/element = %.2f\n",
	   t, cme);
    if (cycp)
      *cycp = cme;
  }
}

/* Compute the grade achieved by function */
static double compute_score(double cmeas, double cref, double cbest)
{
  double sbest = cref/cbest;
  double smeas = cref/cmeas;
  if (smeas < 0.1*(sbest-1)+1)
    return 0;
  if (smeas > 1.1*(sbest-1)+1)
    return 120;
  return 100*((smeas-1.0)/(sbest-1.0) + 0.1);
}

int main()
{
  int i;
  double cme;
  init_tests();
  set_fcyc_clear_cache(1);  /* Set so that clears cache between runs */
  for (i = 0; rc_fun_tab[i].f != NULL; i++) {
      cme = 100.0;
      time_rc(rc_fun_tab[i].f,
	    rc_fun_tab[i].rc_type, rc_fun_tab[i].descr, &cme);
    if (i == 0)
      printf("  Best column sum score   = %.0f\n",
	     compute_score(cme, cstandard[0].cref, cstandard[0].cbest));
    if (i == 1)
      printf("  Best row & column score = %.0f\n",
	     compute_score(cme, cstandard[1].cref, cstandard[1].cbest));
  }
  return 0;
}
