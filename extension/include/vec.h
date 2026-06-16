#ifndef EXTENSION_VEC_H
#define EXTENSION_VEC_H

double vec3_dot(const double a[3], const double b[3]);
double vec3_norm(const double v[3]);
double vec3_distance(const double a[3], const double b[3]);
void   vec3_sub(const double a[3], const double b[3], double out[3]);
void   vec3_scale(const double v[3], double s, double out[3]);
void   vec3_normalize(const double v[3], double out[3]);

#endif