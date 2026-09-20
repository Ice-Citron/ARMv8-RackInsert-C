#include "logging.h"
#include "sim.h"
#include "evaluator.h"

#include <stdio.h>

static void read_geometry(const Sim *sim, EvalGeometry *geom) {
    sim_get_site_pos(sim, "plug_tip"     , geom->plug_tip);
    sim_get_site_pos(sim, "socket_mouth" , geom->socket_mouth);
    sim_get_site_pos(sim, "socket_bottom", geom->socket_bottom);
}

// Console logs 3D-vector information
void print_vec3(const char *label, const double v[3]) {
    printf("%-14s = %.6f %.6f %.6f\n", label, v[0], v[1], v[2]);
}

// Once evaluation run completes, the final state and scores are printed
void print_eval_report(const char *scene_path, const EvalGeometry *geom,
                       const TrialScore *score, double socket_depth) {
    printf("\n\n");
    printf("scene = %s\n", scene_path);
    print_vec3("plug_tip",      geom->plug_tip);
    print_vec3("socket_mouth",  geom->socket_mouth);
    print_vec3("socket_bottom", geom->socket_bottom);

    printf("\n");
    printf("socket_depth    = %.6f\n", socket_depth);
    printf("plug_port_dist  = %.6f\n", score->plug_port_distance);
    printf("lateral_error   = %.6f\n", score->lateral_error);
    printf("axial_depth     = %.6f\n", score->axial_depth);

    printf("\n");
    printf("full_insertion  = %d\n", score->full_insertion);
    printf("partial_insert  = %d\n", score->partial_insertion);

    printf("\n");
    printf("SCORES:\n");
    printf("    tier1 = %.2f\n", score->tier1);
    printf("    tier2 = %.2f\n", score->tier2);
    printf("    tier3 = %.2f\n", score->tier3);
    printf("    total = %.2f\n", score->total);
}

void print_usage_guide(const char *program_name) {
    fprintf(stderr, "ERROR: See proper usage template below:\n");
    fprintf(stderr, "   %s\n", program_name);
    fprintf(stderr, "   %s --scene <path-to-mjcf-scene> --trials <N>\n", 
            program_name);
}

void print_trial_summary(const char *title, const TrialScore *initial, 
                         const TrialScore *final, double plug_motion, 
                         double duration) {
    printf("\n\n%s\n", title);
    printf("    duration                = %.6f\n", duration);
    printf("    plug_motion             = %.6f\n", plug_motion);
    printf("    initial_plug_port_dist  = %.6f\n", initial->plug_port_distance);
    printf("    final_plug_port_dist    = %.6f\n", final->plug_port_distance);
    printf("    final_lateral_error     = %.6f\n", final->lateral_error);
    printf("    final_axial_depth       = %.6f\n", final->axial_depth);
    printf("    final_total_score       = %.6f\n", final->total);
}

// Print MuJoCo's XML loading errors.
void print_load_error(const char *scene_path, const char *error) {
    assert(scene_path != NULL && error != NULL);
    fprintf(stderr, "ERROR: Failed to load MuJoCo's scene: %s\n", scene_path);
    if (error[0] != '\0') {
        fprintf(stderr, "ERROR: [MuJoCo] %s\n", error);
    }
}

// Runs exactly once at start of trial to generate top row of column labels
void write_trace_header(FILE *trace, const Sim *sim) {
    fprintf(trace, "time,state");

    // motor ctrl command signal columns
    for (int i = 0; i < sim->model->nu; i++) {
        fprintf(trace, ",ctrl_%d", i);
    }

    // joint-angle per actuator columns
    for (int i = 0; i < sim->model->nq; i++) {
        fprintf(trace, ",qpos_%d", i);
    }

    fprintf(trace, ",plug_tip_x,plug_tip_y,plug_tip_z,lateral_error,"
                   "axial_depth,plug_port_distance\n");
}

// Runs repeatedly inside `while` loop right after each `sim_step`
void write_trace_row(FILE *trace, const Sim *sim, 
                     const ScriptedPolicy *policy) {
    EvalGeometry geom  = {0};
    TrialScore   score = {0};

    read_geometry(sim, &geom);
    eval_compute_geometry(&geom, &score);

    fprintf(trace, "%.6f,%d", sim->data->time, (int)policy->state);

    // motor ctrl command signal columns
    for (int i = 0; i < sim->model->nu; i++) {
        fprintf(trace, ",%.9f", sim->data->ctrl[i]);
    }

    // joint-angle per actuator columns
    for (int i = 0; i < sim->model->nq; i++) {
        fprintf(trace, ",%.9f", sim->data->qpos[i]);
    }

    fprintf(trace, ",%.9f,%.9f,%.9f,%.9f,%.9f,%.9f\n", 
            geom.plug_tip[0], geom.plug_tip[1], geom.plug_tip[2], 
            score.lateral_error, score.axial_depth, score.plug_port_distance);
}
