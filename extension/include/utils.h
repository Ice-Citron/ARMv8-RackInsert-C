#ifndef EXTENSION_UTILS_H
#define EXTENSION_UTILS_H

#include <assert.h>

static inline double clamp(double x, double lo, double hi) {
    assert(lo <= hi);
    if (x < lo) return lo; 
    if (x > hi) return hi;
    return x;
}

#endif