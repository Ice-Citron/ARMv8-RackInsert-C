#include "evaluator.h"
#include "sim.h"
#include "vec.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SCENE_PATH "assets/mujoco/rack_insert/rack_insert_scene_eval.xml"

// (TEMP Helper): Console logs 3D-vector information
static void print_vec3(const char *label, const double v[3]) {
    printf("%-14s = %.6f %.6f %.6f\n", label, v[0], v[1], v[2]);
}

// (TEMP Helper):
static void print_eval_report(const char *scene_path, const EvalGeometry *geom,
                              const TrialScore *score, double socket_depth) {
    printf("scene = %s\n", scene_path);
    print_vec3("plug_tip",      geom->plug_tip);
    print_vec3("socket_mouth",  geom->socket_mouth);
    print_vec3("socket_bottom", geom->socket_bottom);

    printf("\n");
    printf("socket_depth    = %,6f\n", socket_depth);
    printf("plug_port_dist  = %.6f\n", score->plug_port_distance);
    printf("lateral_error   = %.6f\n", score->lateral_error);
    printf("axial_depth     = %.6f\n", score->axial_depth);

    printf("\n");
    printf("full_insertion  = %d\n", score->full_insertion);
    printf("partial_insert  = %d\n", score->partial_insertion);

    printf("\n");
    printf("SCORES:");
    printf("    tier1 = %.2f\n", score->tier1);
    printf("    tier2 = %.2f\n", score->tier2);
    printf("    tier3 = %.2f\n", score->tier3);
    printf("    total = %.2f\n", score->total);
}

int main(int argc, char **argv) {
    EvalGeometry geom = {
        .plug_tip = {0.0, 0.0, 0.05},
        .socket_mouth = {0.0, 0.0, 0.0},
        .socket_bottom = {0.0, 0.0, 0.10},
    };

    EvalConfig config = eval_default_config();
    TrialScore score = {
        .initial_plug_port_distance = 0.10,
        .duration = 10.0,
        .path_length = 0.10,
        .average_jerk = 1.0,
    };

    eval_compute_geometry(&geom, &score);
    eval_score_trial(&config, 0.10, &score);

    printf("distance=%.6f lateral=%.6f axial=%.6f total=%.2f\n",
           score.plug_port_distance,
           score.lateral_error,
           score.axial_depth,
           score.total);

    return 0;
}
