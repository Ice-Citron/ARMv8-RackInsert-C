#include "evaluator.h"
#include "sim.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_SCENE_PATH "assets/mujoco/rack_insert/rack_insert_scene_eval.xml"

// (TEMP HELPER): Calculates 3D-vector distance. Will move later.
static double vec3_distance(const double ) {
    double dx = a[0] - b[0];
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
