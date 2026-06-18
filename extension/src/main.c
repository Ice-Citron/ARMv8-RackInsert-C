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

    

    sim_free(&sim);
    return EXIT_SUCCESS;
}
