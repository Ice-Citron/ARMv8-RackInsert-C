#include "sim.h"
#include "vec.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_SCENE_PATH \
    "assets/mujoco/rack_insert/rack_insert_scene_cable_softplugin_rollout.xml"
#define MAX_TESTING_SECONDS 8.0     // Timeout Guard for while-loop
#define MIN_PLUG_MOTION_M   1e-5

int main(void) {
    Sim sim;
    sim_load(&sim, DEFAULT_SCENE_PATH);

    EvalConfig config = eval_default_config();

    EvalGeometry initial_geom = {0};
    EvalGeometry final_geom = {0};

    TrialScore initial_score = {0};
    TrialScore final_score = {0};

    read_geometry(&sim, &initial_geom);
    eval_compute_geometry(&initial_geom, &initial_score);

    double socket_depth = vec3_distance(initial_geom.socket_mouth, 
                                        initial_geom.socket_bottom); 
    
    ScriptedPolicy policy;
    scripted_policy_init(&policy, &sim);
    scripted_policy_start(&policy, &sim);

    double previous_tip[3] = {
        initial_geom.plug_tip[0],
        initial_geom.plug_tip[1],
        initial_geom.plug_tip[2],
    };

    double path_length = 0.0;
    
    // Heartbeat of smoke test benchmark. 
    while (!scripted_policy_is_done(&policy) 
           && sim.data.time < MAX_TESTING_SECONDS) {
        // Policy calculates exact motor angles for each ms and sends commands
        scripted_policy_update(&policy, &sim);
        // MuJoCo physics engine ticks forward based on policy's commands
        sim_step(&sim);

        double current_tip[3];
        sim_get_site_pos(&sim, "plug_tip", current_tip);
        
        // For Tier 2 Motion Quality Scoring, to determine path efficiency
        // (lower path length is better, more efficient, don't want arm 
        // zig-zagging unneccesarily).
        path_length += vec3_distance(previous_tip, current_tip);

        previous_tip[0] = current_tip[0];
        previous_tip[1] = current_tip[1];
        previous_tip[2] = current_tip[2];
    }

    read_geometry(&sim, &final_geom);
    eval_compute_geometry(&final_geom, &final_score);

    final_score.initial_plug_port_distance = initial_score.plug_port_distance;
    final_score.path_length = path_length;
    final_score.duration = sim.data->time;
    final_score.average_jerk = 0.0;
    final_score.retries = policy.retries;

    eval_score_trial(&config, socket_depth, &final_score);

    const char *title = "SCRIPTED POLICY ROLLOUT SMOKE TEST"
    double plug_motion = vec3_distance(initial_geom.plug_tip, 
                                       final_geom.plug_tip);
    print_trial_summary(title, &initial_score, &final_score, plug_motion, 
                        sim.data->time);
    
    int policy_finished = scripted_policy_is_done(&policy);
    sim_free(&sim);

    if (!policy_finished) {
        fprintf(stderr, 
                "ERROR: [SMOKE TEST FAILED]: scripted_policy timed out\n");
        return EXIT_FAILURE;
    }
    if (plug_motion < MIN_PLUG_MOTION_M) {
        fprintf(stderr, 
                "ERROR: [SMOKE TEST FAILED]: TCP barely moved\n");
        return EXIT_FAILURE;
    }

    printf("[SMOKE TEST SUCCESS]: scripted_policy completed smoke tests.\n");
    return EXIT_SUCCESS;
}
