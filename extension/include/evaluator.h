#ifndef EXTENSION_EVALUATOR_H
#define EXTENSION_EVALUATOR_H

typedef struct {
    double lateral_tol_m;
    double depth_tol_m;
    double force_threshold_n;
    double force_duration_s;
    double max_duration_s;
} EvalConfig;

typedef struct {
    double lateral_error;
    double axial_depth;
    double plug_port_distance;
    double path_length;
    double duration;
    int    retries;
    int    full_insertion;
    int    partial_insertion;
    double tier1;
    double tier2;
    double tier3;
    double total;
} TrialScore;

typedef struct {
    double plug_tip[3];         // x, y, z
    double socket_mouth[3];     // x, y, z
    double socket_bottom[3];    // x, y, z
} EvalGeometry;

void eval_compute_geometry(const EvalGeometry *geom, TrialScore *score);
void eval_score_trial(const EvalConfig *config, double socket_depth, 
                      TrialScore *score);

#endif
