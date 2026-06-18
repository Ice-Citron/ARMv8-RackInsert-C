#include "sim.h"
#include "vec.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define DEFAULT_SCENE_PATH \
    ""
#define MAX_ROLLOUT_SECONDS 8.0
#define MIN_PLUG_MOTION_M   1e-5

int main() {
    print_trial_summary();
    return EXIT_SUCCESS;
}
