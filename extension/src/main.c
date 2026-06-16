#include "evaluator.h"
#include "sim.h"
#include "vec.h"
#include "logging.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEFAULT_SCENE_PATH "assets/mujoco/rack_insert/rack_insert_scene_eval.xml"

int main(int argc, char **argv) {
    assert(argv[0] != NULL && argc > 0);
    const char *scene_path = DEFAULT_SCENE_PATH;

    if (argc == 3 && strcmp(argv[1], "--scene") == 0) {
        scene_path = argv[2];
    } else if (argc != 1) { // Input error
        const char *program_name = argv[0];
        print_usage_guide(program_name);
        return EXIT_FAILURE;
    }

    Sim sim;
    sim_load(&sim, scene_path);

    EvalGeometry geom = {0};
    sim_get_site_pos(&sim, "plug_tip", geom.plug_tip);
    sim_get_site_pos(&sim, "socket_mouth", geom.socket_mouth);
    sim_get_site_pos(&sim, "socket_bottom", geom.socket_bottom);

    EvalConfig config = eval_default_config();
    TrialScore score = {0};

    eval_compute_geometry(&geom, &score);

    double socket_depth = vec3_distance(geom.socket_mouth, geom.socket_bottom);

    score.initial_plug_port_distance = score.plug_port_distance;
    score.path_length = score.initial_plug_port_distance;
    score.duration = config.max_duration_s;
    score.average_jerk = config.max_jerk_zero_score;
    score.retries = 0;
    
    eval_score_trial(&config, socket_depth, &score);
    print_eval_report(scene_path, &geom, &score, socket_depth);

    sim_free(&sim);
    return EXIT_SUCCESS;
}
