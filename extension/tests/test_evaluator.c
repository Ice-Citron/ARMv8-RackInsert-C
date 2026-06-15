#include "evaluator.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

static void assert_close(double actual, double expected, double tol) {
    assert(fabs(actual - expected) <= tol);
}