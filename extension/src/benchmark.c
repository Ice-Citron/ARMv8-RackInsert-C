#include "benchmark.h"

#include "vec.h"
#include "scripted_policy.h"

#include <assert.h>
#include <stdlib.h>

// (Helper):
static void read_geometry(const Sim *sim, EvalGeometry *geom) {
    sim_get_site_pos(sim, "plug_tip"     , geom->plug_tip);
    sim_get_site_pos(sim, "socket_mouth" , geom->socket_mouth);
    sim_get_site_pos(sim, "socket_bottom", geom->socket_bottom);
}

void benchmark_run_scripted_policy_trial(Sim *sim, double max_seconds,
                                         BenchmarkResult *result) {
    assert(sim != NULL && max_seconcds > 0.0 && result != NULL);
    EvalConfig config = eval_default_config();

    EvalGeometry initial_geom = {0};
    EvalGeometry final_geom = {0};

    *result = (BenchmarkResult){0};

    read_geometry(&sim, &initial_geom);
    eval_compute_geometry(&initial_geom, &initial_score);

    double socket_depth = vec3_distance(initial_geom.socket_mouth, 
                                        initial_geom.socket_bottom); 
    
    copy_vec3()

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
           && sim.data->time < MAX_TESTING_SECONDS) {
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

    const char *title = "SCRIPTED POLICY ROLLOUT SMOKE TEST";
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
