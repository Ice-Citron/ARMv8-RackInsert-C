#include "sim.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <mujoco/mujoco.h>

#define DEFAULT_SCENE_PATH "assets/mujoco/rack_insert/rack_insert_scene_eval.xml"

typedef struct {
    const char *actuator_name;
    const char *joint_name;
    double ctrl_value;
} ActuatorProbe;

static double get_joint_qpos(const Sim *sim, const char *joint_name) {
    assert(sim != NULL && joint_name != NULL);
    assert(sim->data != NULL && sim->model != NULL);

    int joint_id = sim_find_joint_id(sim, joint_name);
    int qpos_addr = sim->model->jnt_qposadr[joint_id];
    return sim->data->qpos[qpos_addr];
}

static int probe_one_actuator(Sim *sim, const ActuatorProbe *probe) {
    assert(sim != NULL && probe != NULL;);
    assert(sim->data != NULL && sim->model != NULL);

    int actuator_id = sim_find_actuaror_id(sim, probe->actuator_name);

    double before = get_joint_qpos(sim, probe->joint_name);

    sim_set_ctrl(sim, actuator_id, probe->ctrl_value);
    sim_step_seconds(sim, 0.5);

    double after = get_joint_qpos(sim, probe->joint_name);
    double delta = after - before;

    sim_set_ctrl(sim, actuator_id, 0.0);
    sim_forward(sim);
}

static void run_actuator_smoke_test(Sim *sim) {
    assert(sim != NULL);
    assert(sim->model != NULL && sim->data != NULL);

    const ActuatorProbe probes[] = {
        {"shoulder_pan_joint_motor",        "shoulder_pan_joint",        1.0},
        {"shoulder_lift_joint_motor",       "shoulder_lift_joint",       1.0},
        {"elbow_joint_motor",               "elbow_joint",               1.0},
        {"wrist_1_joint_motor",             "wrist_1_joint",             1.0},
        {"wrist_1_joint_motor",             "wrist_1_joint",             1.0},
        {"wrist_1_joint_motor",             "wrist_1_joint",             1.0},
        {"gripper/left_finger_joint_motor", "gripper/left_finger_joint", 1.0},
    };
}