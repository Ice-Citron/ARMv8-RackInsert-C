#include "vec.h"

#include <math.h>
#include <assert.h>

// Calculates dot product of 3D vectors
double vec3_dot(const double a[3], const double b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

// Calculates 3D-norm of 3D vector
double vec3_norm(const double v[3]) {
    return sqrt(vec3_dot(v, v));
}

// Calculates 3D-distance between two 3D-vectors
double vec3_distance(const double a[3], const double b[3]) {
    double dist[3];
    vec3_sub(a, b, dist);
    return vec3_norm(dist);
}

// Vector subtraction between two 3D-vectors
void vec3_sub(const double a[3], const double b[3], double out[3]) {
    for (int i = 0; i < 3; i++) {
        out[i] = a[i] - b[i];
    }
}

// Scaling a 3D vector by a scalar multiple
void vec3_scale(const double v[3], double s, double out[3]) {
    for (int i = 0; i < 3; i++) {
        out[i] = s * v[i];
    } 
}

// Normalising a 3D vector
void vec3_normalize(const double v[3], double out[3]) {
    double magnitude = vec3_norm(v);
    assert(magnitude > 0.0);
    for (int i = 0; i < 3; i++) {
        out[i] = v[i] / magnitude;
    }
}

void copy_vec3(const double src[3], double dst[3]) {
    for (int i = 0; i < 3; i++) {
        dst[i] = src[i];
    }
}
