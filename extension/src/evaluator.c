#include "evaluator.h"

#include <math.h>
#include <assert.h>
#include <stddef.h>

// (Helper): Calculates dot product of 3D vectors
static double vec3_dot(const double a[3], const double b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static void vec3_sub(const double a[3], const double b[3], double out[3]) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
}

static double vec3_norm(const double v[3]) {
    return sqrt(vec3_dot(v, v));
}

static double clamp(double x, double lo, double hi) {
    if (x < lo) {
        return lo;
    } 
    if (x > hi) {
        return hi;
    }
    return x;
}

static double descending_linear_score(double value, double full_score_value,
                                      double zero_score_value, 
                                      double max_points) {
    if (value <= full_score_value) {
        return max_points;
    }
    if (value <= zero_score_value) {
        return 0.0;
    }
    double span = zero_score_value - full_score_value;
    if (span <= 0.0) {
        return 0.0;
    }

    // Penalty Slope: "less is more". We want the robot to complete the task
    // with less duration, with lower path length, and less jerkiness.
    return max_points * (zero_score_value - value) / span;
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
    assert(geom != NULL);
    assert(score != NULL);

    double socket_axis[3];
    double plug_rel[3];     // Relative vector of socket mouth to plug tip
    vec3_sub(geom->socket_bottom, geom->socket_mouth, socket_axis);
    vec3_sub(geom->plug_tip, geom->socket_mouth, plug_rel);

    // Normalised socket axis vector to calculate axial depth 
    double socket_axis_length = vec3_norm(socket_axis);
    assert(socket_axis_length > 0.0);
    double norm_axis[3] = {
        socket_axis[0] / socket_axis_length,
        socket_axis[1] / socket_axis_length,
        socket_axis[2] / socket_axis_length
    };
    
    // Dot Product of `Axial Depth` and `Relative Plug Vector` provides
    // axial depth (how far down has the plug traveled into the socket).
    score->axial_depth = vec3_dot(plug_rel, norm_axis);
    score->plug_port_distance = vec3_norm(plug_rel);
    
    // Lateral error: Calculating how far off-center the plug tip is from the
    // socket axis.
    double lateral_vec[3];
    for (int i = 0; i < 3; i++) {
        // Vector pointing to "center" of hole at the plug's current position
        double center_point_at_depth = score->axial_depth * norm_axis[i];
        lateral_vec[i] = plug_rel[i] - center_point_at_depth;
    }
    score->lateral_error = sqrt(vec3_dot(lateral_vec, lateral_vec));
}

void eval_score_trial(const EvalConfig *config, double socket_depth, 
                      TrialScore *score) {
    assert(config != NULL);
    assert(score != NULL);
    assert(socket_depth > 0.0);
    
    score->full_insertion = 0;
    score->partial_insertion = 0;

    // Auto-grants 1.0 pts for trail running to completion without seg-faults
    score->tier1 = 1.0; 
    score->tier2 = 0.0;
    score->tier3 = 0.0;
    score->total = 0.0;

    int laterally_aligned = (score->lateral_error <= config->lateral_tol_m);
    // `socket_depth - config->depth_tol_m` is the finish line.
    if (score->axial_depth >= socket_depth - config->depth_tol_m 
        && laterally_aligned) {
        score->full_insertion = 1;
        // CASE: Full Insertion. Awards full 75 pts.
        score->tier3 = 75.0;
    } else if (laterally_aligned 
               && score->axial_depth > 0.0 
               && score->axial_depth < socket_depth - config->depth_tol_m) {
        // CASE: Partial insertion. Awards base 12 pts, and up to 12 extra pts.
        double depth_fraction = score->axial_depth / socket_depth;
        depth_fraction = clamp(depth_fraction, 0.0, 1.0);

        score->partial_insertion = 1;
        score->tier3 = 38.0 + 12.0 * depth_fraction;
    } else {

    }
}
