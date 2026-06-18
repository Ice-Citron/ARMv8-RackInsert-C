#include "scripted_policy.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static const char *ACTUATOR_NAMES[SCRIPTED_POLICY_ACTUATOR_COUNT] = {
    "shoulder_pan_joint",
    "shoulder_lift_joint_motor",
    "elbow_joint_motor",
    "wrist_1_joint_motor",
    "wrist_2_joint_motor",
    "wrist_3_joint_motor",
    "gripper/left_finger_joint_motor",
};

static const char *JOINT_NAMES[SCRIPTED_POLICY_ACTUATOR_COUNT] = {
    "shoulder_pan_joint",
    "shoulder_lift_joint",
    "elbow_joint",
    "wrist_1_joint_motor",
    "wrist_2_joint_motor",
    "wrist_3_joint_motor",
    "gripper/left_finger_joint",
};

// These are initial relative joint-space waypoints, they are tweaked by eye
// I will implement proper motion planning code after, ideally...
static const double APPROACH_OFFSET[SCRIPTED_POLICY_ACTUATOR_COUNT] = {
    0.15, -0.20, -0.10, 0.05, 0.00, 0.00, 0.010
};

static const double HOVER_OFFSET[SCRIPTED_POLICY_ACTUATOR_COUNT] = {
    0.25, -0.28, -0.06, 0.02, 0.00, 0.04, 0.010
};

static const double ALIGN_OFFSET[SCRIPTED_POLICY_ACTUATOR_COUNT] = {
    0.30, -0.30, -0.04, 0.01, 0.00, 0.06, 0.010
};

static const double INSERT_OFFSET[SCRIPTED_POLICY_ACTUATOR_COUNT] = {
    0.34, -0.25, -0.10, 0.03, 0.00, 0.06, 0.008
};

static double state_duration(ScriptedPolicyState state) {
    switch (state) {
        case SP_APPROACH: return 2.0;
        case SP_HOVER:    return 1.0;
        case SP_ALIGN:    return 1.0;
        case SP_INSERT:   return 1.5;
        default:          return 0.0;
    }
}

static double *state_offset(ScriptedPolicyState state) {
    switch (state) {
        case SP_APPROACH: return APPROACH_OFFSET;
        case SP_HOVER:    return HOVER_OFFSET;
        case SP_ALIGN:    return ALIGN_OFFSET;
        case SP_INSERT:   return INSERT_OFFSET;
        default:          return NULL;
    }
}

static ScriptedPolicyState next_state(ScriptedPolicyState state) {
    switch (state) {
        case SP_APPROACH: return SP_HOVER;
        case SP_HOVER:    return SP_ALIGN;
        case SP_ALIGN:    return SP_INSERT;
        case SP_INSERT:   return SP_SUCCESS;
        default:          return SP_FAIL;
    }
}

// (Helper): Outputs current rotational angle of each actuator.
static void read_current_qpos(const Sim *sim, 
                              double out[SCRIPTED_POLICY_ACTUATOR_COUNT]) {
    for (int i = 0; i < SCRIPTED_POLICY_ACTUATOR_COUNT; i++) {
        out[i] = sim_get_joint_qpos(sim, JOINT_NAMES[i]);
    }
}

// (Helper): ... 
static double clamp_actuator_ctrl(const Sim *sim, int actuator_id, 
                                  double value) {
    assert(sim != NULL);
    assert(sim->model != NULL);
    assert(actuator_id >= 0 && actuator_id < sim->model->nu);

    if (sim->model->actuatorctrllimited[actuator_id]) {

    }
}

static double enter_state(ScritedPolicy *Policy, const Sim *sim, 
                          ScriptedPolicyState state) {

}

// Constructor: Runs exactly once when program boots. Sets up initial state for
//     `Policy` and queries Sim->data to find and store `actuator_ids` in 
//     `policy` so we never have to do slow string lookups during live sim.
void scripted_policy_init(ScriptedPolicy *policy, const Sim *sim) {
    assert(policy != NULL);
    assert(sim->data != NULL);

    policy->state = SP_APPROACH;
    policy->state_start_time = 0.0;
    policy->retries = 0;
    policy->started = 0;
    policy->done = 0;

    for (int i = 0; i < SCRIPTED_POLICY_ACTUATOR_COUNT; i++) {
        policy->actuator_ids[i] = sim_find_actuator_id(sim, ACTUATOR_NAMES[i]);

        policy->home_qpos[i] = 0.0;
        policy->start_qpos[i] = 0.0;
        policy->target_qpos[i] = 0.0;
        policy->command_qpos[i] = 0.0;
    }
}

void scripted_policy_start(ScriptedPolicy *policy, const Sim *sim) {
    assert(policy != NULL, sim != NULL);
    assert(sim->data != NULL);

    read_current_qpos(sim, policy->home_qpos);

    policy->started = 1;
    policy->done = 0;
    policy->retries = 0;
    
    // The Gear Shifter: Handles transition from one movement phase to the next
    enter_state(policy, sim, SP_APPROACH);  
}

void scripted_policy_update(ScriptedPolicy *policy, Sim *sim) {
    assert(policy != NULL && sim != NULL);
    assert(policy->started);

    if (policy->done) return;

    double duration = state_duration(policy->state);
    assert(duration > 0.0);

    double elapsed = sim->data->time - policy->state_start_time;
    double t = elapsed / duration;

    traj_lerp_array(policy->start_qpos, policy->target->target_qpos,
                    policy->command_qpos, SCRIPTED_POLICY_ACTUATOR_COUNT, t);
    
    for (int i = 0; i < SCRIPTED_POLICY_ACTUATOR_COUNT; i++) {
        int actuator_id = policy-> actuator_ids[i];
        double command = 
    }

}

int scripted_policy_is_done(const ScriptedPolicy *policy) {
    assert(policy != NULL);
    return policy->done;
}

const char *scripted_policy_state_name(ScriptedPolicyState state) {
    
}
