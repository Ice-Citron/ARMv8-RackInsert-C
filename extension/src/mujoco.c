#include <mujoco/mujoco.h>
#include <math.h>

static void vec3_sub(double a[3], double b[3], double out[3]) {
    for (int i = 0; i < 3; i++) {
        out[i] = a[i] - b[i];
    }
}

static double vec3_dot(double a[3]) {
    return a[0]*a[0] + a[1]*a[1] + a[2]*a[2];
}

static double vec3_norm(double a[3]) {
    return sqrt(vec3_dot(a));
}

void compute_axis(double mouth[3], double bottom[3], double axis[3], double *depth) {
    vec3_sub(bottom, mouth, axis);
    
    *depth = vec3_norm(axis);
    if (depth <= 0.0) {
        fprintf(stderr, "ERROR: Invalid socket depth.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++) {
        axis[i] /= *depth;
    }
}