#include "utils.h"

#include <assert.h>
#include <stdlib.h>

// Linear interpolation (t is normalised, in [0, 1])
double traj_lerp(double start, double end, double t) {
    t = clamp(t, 0.0, 1.0);
    return start + t * (end - start);
}

// S-curve polynomial 3t^2 - 2t^3 to prevent jerk.
double traj_smoothstep(double t) {
    t = clamp(t, 0.0, 1.0);
    return 3.0*t*t - 2.0*t*t*t;
}

void traj_copy_array(const double *src, double *dst, int count) {
    assert(src != NULL && dst != NULL && count >= 0);

    for (int i = 0; i < count; i++) {
        dst[i] = src[i];
    }
}

void traj_lerp_array(const double *start, const double *goal, double *out, 
                     int count, double t) {
    assert(start != NULL && goal != NULL && out != NULL && count >= 0);

    double s = traj_smoothstep(t);
    for (int i = 0; i < count; i++) {
        out[i] = traj_lerp(start[i], end[i], s);
    }
}
