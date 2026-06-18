#include "scripted_policy.h"
#include "trajectory.h"
#include "utils.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static const char *ACTUATOR_NAMES[SCRIPTED_POLICY_ACTUATOR_COUNT] = {
    "shoulder_pan_joint_motor",
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
    "wrist_1_joint",
    "wrist_2_joint",
    "wrist_3_joint",
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

static const double *state_offset(ScriptedPolicyState state) {
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

// (Helper): Checks the MuJoCO model to see if the requested motor has physical
//      control limits (e.g. motor A might only be able to rotate between -0.5r
//      and 1.0r, hence, `ctrlrange=-0.5 1.0`). If limits exist, it clamps the
//      requested actuation value to ensure the motors are never commanded to
//      exceed its safe operating range.
static double clamp_actuator_ctrl(const Sim *sim, int actuator_id, 
                                  double value) {
    assert(sim != NULL);
    assert(sim->model != NULL);
    assert(actuator_id >= 0 && actuator_id < sim->model->nu);

    if (sim->model->actuator_ctrllimited[actuator_id]) {
        // Physical limit of actuation range of requested motor.
        double lo = sim->model->actuator_ctrlrange[2 * actuator_id + 0];
        double hi = sim->model->actuator_ctrlrange[2 * actuator_id + 1];
        return clamp(value, lo, hi);
    }
    // Returns unchanged actuation value if ctrlrange not found
    return value;
}

// (Helper): The Gear Shifter. Handles robot's transition from one FSM state
//      to the next.
static void enter_state(ScriptedPolicy *policy, const Sim *sim, 
                          ScriptedPolicyState state) {
    assert(policy != NULL && sim != NULL);

    // Hand-tweaked relative offset for robot's TCP to go to.
    const double *offset = state_offset(state);

    // Updates state in `policy` to keep track
    policy->state = state;
    policy->state_start_time = sim->data->time;

    if (state == SP_SUCCESS || state == SP_FAIL) {
        policy->done = 1;
    } else {
        assert(offset != NULL);
        read_current_qpos(sim, policy->start_qpos);

        for (int i = 0; i < SCRIPTED_POLICY_ACTUATOR_COUNT; i++) {
            // Calculates absolute destination (angle) for this new FSM state
            policy->target_qpos[i] = policy->home_qpos[i] + offset[i];
        }
    }

    printf("LOGGING: Scripted Policy is now entering [%s] state\n",
           scripted_policy_state_name(state));
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

// Starter: RUns at the start of evey single insertion trial.
void scripted_policy_start(ScriptedPolicy *policy, const Sim *sim) {
    assert(policy != NULL && sim != NULL);
    assert(sim->data != NULL);

    read_current_qpos(sim, policy->home_qpos);

    policy->started = 1;
    policy->done = 0;
    policy->retries = 0;
    
    // The Gear Shifter: Handles transition from one movement phase to the next
    enter_state(policy, sim, SP_APPROACH);  
}

// Heartbeat: This function is called every single time in  `for` loop
//      when `mj_step` is called.
void scripted_policy_update(ScriptedPolicy *policy, Sim *sim) {
    assert(policy != NULL && sim != NULL);
    assert(policy->started);

    if (policy->done) return;

    // To check how much longer will policy spend in current FSM state
    double duration = state_duration(policy->state);
    assert(duration > 0.0);
    double elapsed = sim->data->time - policy->state_start_time;
    double t = elapsed / duration;

    // Uses Lerp to understand what angles should each actuators hold at current
    // the millisecond.
    traj_lerp_array(policy->start_qpos, policy->target_qpos,
                    policy->command_qpos, SCRIPTED_POLICY_ACTUATOR_COUNT, t);
    
    for (int i = 0; i < SCRIPTED_POLICY_ACTUATOR_COUNT; i++) {
        int actuator_id = policy-> actuator_ids[i];
        double command = clamp_actuator_ctrl(sim, actuator_id, 
                                             policy->command_qpos[i]);
        sim_set_ctrl(sim, actuator_id, command);
    }

    if (t >= 1.0) {
        // If designated state duration exceeded, FSM switches over to the next
        // movement phase. 
        enter_state(policy, sim, next_state(policy->state));
    }
}

// (Utility): Returns `1` or `0` so main `while` loop knows if robot has 
//      finished the entire insertion attempt.
int scripted_policy_is_done(const ScriptedPolicy *policy) {
    assert(policy != NULL);
    return policy->done;
}

// (Utility): Translates int-state FSM enums into human-readable strings for 
//      debug print logs.
const char *scripted_policy_state_name(ScriptedPolicyState state) {
    switch (state) {
        case SP_APPROACH:   return "APPROACH";
        case SP_HOVER:      return "HOVER";
        case SP_ALIGN:      return "ALIGN";
        case SP_INSERT:     return "INSERT";
        case SP_SUCCESS:    return "SUCCESS";
        case SP_FAIL:       return "FAIL";
        default:
            fprintf(stderr, "ERROR: Invalid ScriptedPolicyState (FSM) state\n");
            exit(EXIT_FAILURE);
;    }   
}
