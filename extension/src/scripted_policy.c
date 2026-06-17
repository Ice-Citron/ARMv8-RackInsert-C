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
        policy->
    }
}

void scripted_policy_start(ScriptedPolicy *policy, const Sim *sim) {

}

void scripted_policy_update(ScriptedPolicy *policy, Sim *sim) {

}

int scripted_policy_is_done(const ScriptedPolicy *policy) {
    assert(policy != NULL);
    return policy->done;
}

const char *scripted_policy_state_name(ScriptedPolicyState state) {
    
}
