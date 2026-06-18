#ifndef EXTENSION_BENCHMARK_H
#define EXTENSION_BENCHMARK_H

#include "sim.h"
#include "evaluator.h"

// Transferred and consolidated from test_script_policy.c
typedef struct {
    TrialScore initial_score;
    TrialScore final_score;

    double initial_plug_tip[3];
    double final_plug_tip[3];
    double socket_depth;
    double plug_motion;
    double duration;
    double path_length;

    int policy_finished;
} BenchmarkResult;

void benchmark_run_scripted_policy_trial(Sim *sim, double max_seconds,
                                         BenchmarkResult *result);

#endif
