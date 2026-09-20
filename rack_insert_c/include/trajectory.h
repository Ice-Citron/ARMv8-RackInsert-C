#ifndef EXTENSION_TRAJECTORY_H
#define EXTENSION_TRAJECTORY_H

double traj_lerp(double a, double b, double t);
double traj_smoothstep(double t);

void traj_copy_array(const double *src, double *dst, int count);
void traj_lerp_array(const double *start, const double *goal, double *out, 
                     int count, double t);

#endif