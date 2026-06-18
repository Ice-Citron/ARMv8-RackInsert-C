#include "sim.h"
#include "logging.h"
#include "benchmark.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define DEFAULT_SCENE_PATH \
    "assets/mujoco/rack_insert/rack_insert_scene_rollout.xml"
#define DEFAULT_MAX_SECONDS 8.0

int main(int argc, char **argv) {
    assert(argv[0] != NULL && argc > 0);

    const char *scene_path = DEFAULT_SCENE_PATH;
    int trials = 1;

    // Dynamic CLI parser which allows user to set --scene and --trials
    //    - Starts from i=1 because i=0 is char *program_name
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--scene")) {
            if (i + 1 >= argc) {
                print_usage_guide(argv[0]);
                return EXIT_FAILURE;
            }
            scene_path = argv[++i];
        } else if (strcmp(argv[i], "--trials") == 0) {
            int parse_success = parse_int_arg(argv[i + 1], &trials);
            if (i + 1 >= argc || !parse_success) {
                print_usage_guide(argv[0]);   // argv[0] == char *program_name
                return EXIT_FAILURE;
            }
            i++;
        } else { // If doesn't parse to `--scene` or `--trials`
            print_usage_guide(argv[0]);
            return EXIT_FAILURE;
        }
    }

    sim_free(&sim);
    return EXIT_SUCCESS;
}
