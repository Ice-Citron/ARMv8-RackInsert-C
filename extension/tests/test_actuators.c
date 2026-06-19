#include "sim.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mujoco/mujoco.h>

#define DEFAULT_SCENE_PATH \
    "assets/mujoco/rack_insert/rack_insert_scene_cable_softplugin_rollout.xml"

typedef struct {
    const char *actuator_name;
    const char *joint_name;
    double ctrl_value;
} ActuatorProbe;

static int probe_one_actuator(Sim *sim, const ActuatorProbe *probe) {
    assert(sim != NULL && probe != NULL);
    assert(sim->data != NULL && sim->model != NULL);

    int actuator_id = sim_find_actuator_id(sim, probe->actuator_name);

    double before = sim_get_joint_qpos(sim, probe->joint_name);

    sim_set_ctrl(sim, actuator_id, probe->ctrl_value);
    sim_step(sim);

    double after = sim_get_joint_qpos(sim, probe->joint_name);
    double delta = after - before;

    sim_set_ctrl(sim, actuator_id, 0.0);
    sim_forward(sim);

    printf("%-32s joint=%-28s before=%.6f after=%.6f delta=%.6f",
           probe->actuator_name, probe->joint_name, before, after, delta);

    if (fabs(delta) > 1e-5) {
        printf("    MOVED\n");
        return 1;
    } else {
        printf("    NO_MOVE\n");
        return 0;
    }
}

int main(void) {
    const ActuatorProbe probes[] = {
        {"shoulder_pan_joint_motor",        "shoulder_pan_joint",        1.0},
        {"shoulder_lift_joint_motor",       "shoulder_lift_joint",       1.0},
        {"elbow_joint_motor",               "elbow_joint",               1.0},
        {"wrist_1_joint_motor",             "wrist_1_joint",             1.0},
        {"wrist_2_joint_motor",             "wrist_2_joint",             1.0},
        {"wrist_3_joint_motor",             "wrist_3_joint",             1.0},
        {"gripper/left_finger_joint_motor", "gripper/left_finger_joint", 1.0},
    };

    const int probe_count = (int)(sizeof(probes) / sizeof(probes[0]));

    Sim sim;
    sim_load(&sim, DEFAULT_SCENE_PATH);

    printf("\n\nACTUATOR SMOKE TEST\n");
    printf("    scene    = %s\n", DEFAULT_SCENE_PATH);
    printf("    model.nu = %ld\n\n", sim.model->nu);

    /*
    // Instabilities are happening to the scene currently. [PROBLEM SOLVED!]
    printf("PASSIVE STEP TEST\n");
    for (int i = 0; i < 10; i++) {
        sim_step(&sim);
        printf("step=%d time=%.6f\n", i + 1, sim.data->time);
    }
    printf("PASSIVE STEP TEST COMPLETE\n");
    */

    int moved_count = 0;
    for (int i = 0; i < probe_count; i++) {
        moved_count += probe_one_actuator(&sim, &probes[i]);
    }
    printf("\nActuators Moved: %d out of %d\n", moved_count, probe_count);

    sim_free(&sim);

    if (moved_count == 0) {
        fprintf(stderr, "SMOKE TEST FAILED: no actuators incurred joint "
                "motion\n\n\n");
        return EXIT_FAILURE;
    } else {
        printf("SMOKE TEST PASSED: at least one actuator incurred joint "
               "motion\n\n\n");
        return EXIT_SUCCESS;
    }
}