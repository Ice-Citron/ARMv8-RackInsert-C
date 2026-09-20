#include "benchmark.h"

#include "vec.h"
#include "scripted_policy.h"
#include "logging.h"

#include <assert.h>
#include <stdlib.h>

static void read_geometry(const Sim *sim, EvalGeometry *geom) {
    sim_get_site_pos(sim, "plug_tip"     , geom->plug_tip);
    sim_get_site_pos(sim, "socket_mouth" , geom->socket_mouth);
    sim_get_site_pos(sim, "socket_bottom", geom->socket_bottom);
}

void benchmark_run_scripted_policy_trial_trace(Sim *sim, double max_seconds,
                                               const char *trace_path,
                                               BenchmarkResult *result) {
    assert(sim != NULL && max_seconds > 0.0 && result != NULL);
    EvalConfig config = eval_default_config();

    EvalGeometry initial_geom = {0};
    EvalGeometry final_geom = {0};

    *result = (BenchmarkResult){0};

    read_geometry(sim, &initial_geom);
    eval_compute_geometry(&initial_geom, &result->initial_score);

    result->socket_depth = vec3_distance(initial_geom.socket_mouth, 
                                         initial_geom.socket_bottom); 
    
    // Writing `BenchmarkResult *result`'s contents.    
    copy_vec3(initial_geom.plug_tip, result->initial_plug_tip);

    ScriptedPolicy policy;
    scripted_policy_init(&policy, sim);
    scripted_policy_start(&policy, sim);

    double previous_tip[3];
    copy_vec3(initial_geom.plug_tip, previous_tip);

    // For recording of trajectory in designated file path
    FILE *trace = NULL;
    if (trace_path != NULL) {
        trace = fopen(trace_path, "w");
        if (trace == NULL) {
            fprintf(stderr, "ERROR: Failed to record data in %s\n", trace_path);
            exit(EXIT_FAILURE);
        }
        write_trace_header(trace, sim);
    }

    // Start time specific to each trial
    double start_time = sim->data->time;

    // Heartbeat of smoke test benchmark. 
    while (!scripted_policy_is_done(&policy) 
           && sim->data->time - start_time < max_seconds) {
        // Policy calculates exact motor angles for each ms and sends commands
        scripted_policy_update(&policy, sim);
        // MuJoCo physics engine ticks forward based on policy's commands
        sim_step(sim);
        
        if (trace != NULL) write_trace_row(trace, sim, &policy);

        double current_tip[3];
        sim_get_site_pos(sim, "plug_tip", current_tip);
        
        // For Tier 2 Motion Quality Scoring, to determine path efficiency
        // (lower path length is better, more efficient, don't want arm 
        // zig-zagging unneccesarily).
        result->path_length += vec3_distance(previous_tip, current_tip);
        copy_vec3(current_tip, previous_tip);
    }

    if (trace != NULL) fclose(trace);

    read_geometry(sim, &final_geom);
    eval_compute_geometry(&final_geom, &result->final_score);

    // `TrialScore result->final_score` specific stats
    result->final_score.initial_plug_port_distance = 
        result->initial_score.plug_port_distance;
    result->final_score.path_length  = result->path_length;
    result->final_score.duration     = sim->data->time - start_time;
    result->final_score.average_jerk = 0.0;
    result->final_score.retries      = policy.retries;

    eval_score_trial(&config, result->socket_depth, &result->final_score);

    copy_vec3(final_geom.plug_tip, result->final_plug_tip);

    // `BenchmarkResult` specific stats
    result->plug_motion = vec3_distance(initial_geom.plug_tip, 
                                        final_geom.plug_tip);
    result->duration = sim->data->time - start_time;
    result->policy_finished = scripted_policy_is_done(&policy);
}

void benchmark_run_scripted_policy_trial(Sim *sim, double max_seconds,
                                         BenchmarkResult *result) {
    benchmark_run_scripted_policy_trial_trace(sim, max_seconds, NULL, result);
}
