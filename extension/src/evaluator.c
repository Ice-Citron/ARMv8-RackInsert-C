#include "evaluator.h"

#include <stdio.h>
#include <math.h>

// (Helper): Calculates dot product of 3D vectors
static double vec3_dot(const double a[3], const double b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static double vec3_sub(const double a[3], const double b[3], double out[3]) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
}

static double vec3_norm(const double v[3]) {
    return sqrt(vec3_dot(v, v));
}

EvalConfig eval_default_config(void) {
    return (EvalConfig) {
        .lateral_tol_m = 0.005,
        .depth_tol_m   = 0.002,
        .min_duration_full_score_s = 5.0,
        .max_duration_s = 60.0,
        .max_jerk_full_score = 0.0,
        .max_jerk_zero_score = 50.0,
        .force_threshold_n = 20.0,
        .force_duration_threshold_s = 1.0,
        .retry_penalty = 2.0,
        .offlimit_contact_penalty = 24.0,
    };
}

void eval_compute_geometry(const EvalGeometry *geom, TrialScore *score) {
    double dx = geom->plug_tip[0] - geom->socket_mouth[0];
    double dy = geom->plug_tip[1] - geom->socket_mouth[1];
    double dz = geom->plug_tip[2] - geom->socket_mouth[2];
    score->plug_port_distance = sqrt(dx*dx + dy*dy + dz*dz);

    double socket_axis[3];
    double plug_rel[3];     // Relative vector of socket mouth to plug tip
    for (int i = 0; i < 3; i++) {
        socket_axis[i] = geom->socket_bottom[i] - geom->socket_mouth[i];
        plug_rel[i]    = geom->plug_tip[i] - geom->socket_mouth[i];
    }

    // Normalised socket axis vector to calculate axial depth 
    double norm_axis[3];
    double socket_axis_length = sqrt(socket_axis[0]*socket_axis[0] + 
                                     socket_axis[1]*socket_axis[1] + 
                                     socket_axis[2]*socket_axis[2]);
    for (int i = 0; i < 3; i++) {
        norm_axis[i] = socket_axis[i] / socket_axis_length;
    }
    // Dot Product of `Axial Depth` and `Relative Plug Vector` provides
    // axial depth (how far down has the plug traveled into the socket).
    score->axial_depth = (plug_rel[0] * norm_axis[0]) +
                         (plug_rel[1] * norm_axis[1]) +
                         (plug_rel[2] * norm_axis[2]);

    // Lateral error: Calculating how far off-center the plug tip is from the
    // socket axis.
    double lateral_vec[3];
    for (int i = 0; i < 3; i++) {
        // Vector pointing to "center" of hole at the plug's current position
        double center_point_at_depth = score->axial_depth * norm_axis[i];
        lateral_vec[i] = plug_rel[i] - center_point_at_depth;
    }
    score->lateral_error = sqrt(lateral_vec[0]*lateral_vec[0] +
                                lateral_vec[1]*lateral_vec[1] +
                                lateral_vec[2]*lateral_vec[2]);
}

void eval_score_trial(const EvalConfig *config, double socket_depth, 
                      TrialScore *score) {
    // 



}
