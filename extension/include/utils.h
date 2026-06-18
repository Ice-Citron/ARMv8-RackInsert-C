#ifndef EXTENSION_UTILS_H
#define EXTENSION_UTILS_H

#include <assert.h>
#include <stdlib.h>

static inline double clamp(double x, double lo, double hi) {
    assert(lo <= hi);
    if (x < lo) return lo; 
    if (x > hi) return hi;
    return x;
}

static int parse_int_arg(const char *text, int *out) {
    assert(text != NULL && out != NULL);

    char *end = NULL;
    long value = strtol(text, &end, 10);

    if (end == text || *end != '\0' || value <= 0) {
        return 0;
    }

    *out = (int)value;
    return 1;
}

#endif