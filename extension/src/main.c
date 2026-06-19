#include "sim.h"
#include "logging.h"
#include "benchmark.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEFAULT_SCENE_PATH \
    "assets/mujoco/rack_insert/rack_insert_scene_cable_softplugin_rollout.xml"
#define DEFAULT_RESULTS_CSV_PATH "results.csv"
#define DEFAULT_MAX_SECONDS 8.0

int main(int argc, char **argv) {
    assert(argv[0] != NULL && argc > 0);

    const char *scene_path = DEFAULT_SCENE_PATH;
    const char *trace_path = DEFAULT_RESULTS_CSV_PATH;  // Path to store recorded trajectory csv
    int trials = 1;

    // Dynamic CLI parser which allows user to set --scene and --trials
    //    - Starts from i=1 because i=0 is char *program_name
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--scene") == 0) {
            if (i + 1 >= argc) {
                print_usage_guide(argv[0]);
                return EXIT_FAILURE;
            }
            scene_path = argv[++i];     // Parses path
        } else if (strcmp(argv[i], "--trials") == 0) {
            if (i + 1 >= argc || !parse_int_arg(argv[i + 1], &trials)) {
                print_usage_guide(argv[0]);   // argv[0] == char *program_name
                return EXIT_FAILURE;
            }
            i++;
        } else if (strcmp(argv[i], "--trace") == 0) {
            if (i + 1 >= argc) {
                print_usage_guide(argv[0]);
                return EXIT_FAILURE;
            }
            trace_path = argv[++i];     // Parses path
        } else { // If doesn't parse to `--scene` or `--trials`
            print_usage_guide(argv[0]);
            return EXIT_FAILURE;
        }
    }

    double total_score = 0.0;
    int completed_trials = 0;

    printf("\n\nRACK INSERT BENCHMARK\n");
    printf("\tscene  = %s\n", scene_path);
    printf("\ttrials = %d\n", trials);
    
    // Logging trace path
    if (trace_path != NULL)
        printf("\ttrace  = %s\n", trace_path);
    if (trace_path != NULL && trials != 1)
        printf("\033[33mNOTE: --trace currently only supports exactly one "
               "trial.\033[0m\n");

    for (int trial = 0; trial < trials; trial++) {
        Sim sim;
        sim_load (&sim, scene_path);

        BenchmarkResult result;
        const char *trial_trace_path = (trial == 0) ? trace_path : NULL;    // Currently trace only supports exactly one trial
        benchmark_run_scripted_policy_trial_trace(&sim, DEFAULT_MAX_SECONDS, 
                                                  trial_trace_path, &result);

        const char *title = "SCRIPTED POLICY BENCHMARK";
        printf("\nTRIAL %d/%d\n", trial + 1, trials);
        print_trial_summary(title, &result.initial_score, &result.final_score,
                            result.plug_motion, result.duration);

        if (result.policy_finished) { completed_trials++; }
        
        total_score += result.final_score.total;
        sim_free(&sim);
    }

    printf("\nBENCHMARK SUMMARY\n");
    printf("\tcompleted_trials = %d/%d\n", completed_trials, trials);
    printf("\ttotal_score      = %.2f\n",  total_score);
    printf("\tmean_score       = %.2f\n",  total_score / (double)trials);

    return EXIT_SUCCESS;
}
