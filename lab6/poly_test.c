/* Test setup for polynomial evaluation.  Do not change this. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "poly.h"
#include "cpe.h"

/* Degree for fixed evaluation */
#define FIXDEGREE 10
/* Largest degree polynomial tested */
#define MAXDEGREE 1000
static int coeff[MAXDEGREE+1];

#define DEFAULT_TEST 1000
static int test_degree = DEFAULT_TEST;

/* Define performance standards */
static struct {
  double cref;  /* Cycles taken by reference solution */
  double cbest; /* Cycles taken by our best implementation */
} cstandard[2] = 
{{11.00, 2.45}, /* CPE */
 {160.0, 135.0} /* C(10) */
};

/* Should I print extra information? */
int verbose = 0;

/* Standard value for polynomial evaluation */
static int xval;

/* How many degrees should I compute reference value for? */
#define DCNT 20

/* Correct value of polynomial evaluation for range of different degrees */
/* pval[i] contains evaluation for degree test_degree-i */
static int pval[DCNT];
/* fixval contains evaluation for degree FIXDEGREE */
static int fixval;

/* Reference implementation */
static int ref_poly_eval(int *a, int degree, int x)
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

/* Initialize polynomial to random values and compute reference values */
static void init()
{
    int i;
    xval = random();
    for (i = 0; i <= test_degree; i++)
	coeff[i] = random();
    for (i = 0; i < DCNT; i++)
	pval[i] = ref_poly_eval(coeff, test_degree-i, xval);
    fixval = ref_poly_eval(coeff, FIXDEGREE, xval);
}

/* Test function on standard test cases. */
int test_poly(peval_fun f, FILE *rpt) {
    int i;
    int v;
    int ok = 1;
    for (i = 0; i < DCNT; i++) {
	v = f(coeff, test_degree-i, xval);
	if (v != pval[i]) {
	    ok = 0;
	    if (rpt) {
		fprintf(rpt, 
 "ERROR.  Polynomial eval. for degree %d gives value %d.  Should give %d\n",
			test_degree-i, v, pval[i]);
	    }
	}
    }
    v = f(coeff, FIXDEGREE, xval);
    if (v != fixval) {
	ok = 0;
	if (rpt) {
	    fprintf(rpt, 
    "ERROR.  Polynomial eval. for degree %d gives value %d.  Should give %d\n",
		    FIXDEGREE, v, fixval);
	}
    }
    return ok;
}

/* Fit into framework of cpe measuring code */
static peval_fun pfun;

volatile int sink;
/* Run pfun for given degree */
void run_fun(int degree)
{
    sink = pfun(coeff, degree, xval);
}


/* Test and measure polynomial evaluation function.  Set values
   of CPE and CFIX */
void run_poly(peval_fun f, char *descr, double *cpep, double *cfixp)
{
    pfun = f;
    printf("Function %s\n", descr);
    if (test_poly(f, stdout)) {
	double cpe = 
	    find_cpe_full(run_fun, test_degree, 300, 
			  stdout, RAN_SAMPLE, 0.2, 0);
	double fix_time = measure_function(run_fun, FIXDEGREE);
	if (verbose)
	    printf("   CPE = %.2f\tOverhead = %.2f\tC(%d) = %.1f\n", cpe,
		   last_overhead, FIXDEGREE, fix_time);
	else
	    printf("   CPE = %.2f\tC(%d) = %.1f\n", cpe,
		   FIXDEGREE, fix_time);
	if (cpep)
	  *cpep = cpe;
	if (cfixp)
	  *cfixp = fix_time;
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

int main(int argc, char *argv[])
{
  int i;
  double cpe = cstandard[0].cref;
  double cfix = cstandard[1].cref;
  if (argc > 1 && !strcmp(argv[1], "-v"))
      verbose = 1;
  if (argc > 1) {
    test_degree = atoi(argv[1]);
    if (test_degree < 1) {
      fprintf(stderr, "Must specify maximum degree > 1\n");
      exit(1);
    }
    if (test_degree > MAXDEGREE) {
      fprintf(stderr, "Must specify maximum degree <= %d\n", MAXDEGREE);
      exit(1);
    }
  }
  init();
  for (i = 0; peval_fun_tab[i].f != NULL; i++) {
    run_poly(peval_fun_tab[i].f, peval_fun_tab[i].descr, &cpe, &cfix);
    if (i == 0)
      printf("  Best CPE score = %.0f\n",
	     compute_score(cpe, cstandard[0].cref, cstandard[0].cbest));
    if (i == 1)
      printf("  Best C(10) score = %.0f\n",
	     compute_score(cfix, cstandard[1].cref, cstandard[1].cbest));
  }
  return 0;
}

