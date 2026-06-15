#include "evaluator.h"

#include <stdio.h>

int main(int argc, char **argv) {
    EvalGeometry geom = {
        .plug_tip = {0.0, 0.0, 0.05},
        .socket_mouth = {0.0, 0.0, 0.0},
        .socket_bottom = {0.0, 0.0, 0.10},
    };

    EvalConfig config = eval_default_config();
    TrialScore score = {
        .inital_plug_port_distance = 0.10;
        .duration = 10.0;
        .path_length = 0.10;
        .average_jerk = 1.0;
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
