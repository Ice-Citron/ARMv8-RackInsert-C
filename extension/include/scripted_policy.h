#ifndef EXTENSION_SCRIPTED_POLICY
#define EXTENSION_SCRIPTED_POLICY

#include "sim.h"

#define SCRIPTED_POLICY_ACTUATOR_COUNT 7

typedef enum {
    SP_APPROACH = 0,
    SP_HOVER,
    SP_ALIGN,
    SP_INSERT,
    SP_SUCCESS,
    SP_FAIL
} ScriptedPolicyState;

typedef struct {
    ScriptedPolicyState state;

    int actuator_ids[SCRIPTED_POLICY_ACTUATOR_COUNT];

    double home_qpos[SCRIPTED_POLICY_ACTUATOR_COUNT];
    double start_qpos[SCRIPTED_POLICY_ACTUATOR_COUNT];
    double target_qpos[SCRIPTED_POLICY_ACTUATOR_COUNT];
    double command_qpos[SCRIPTED_POLICY_ACTUATOR_COUNT];

    double state_start_time;
    int retries;
    int started;
    int done;
} ScriptedPolicy;

void scripted_policy_init(ScriptedPolicy *policy, const Sim *sim);
void scripted_policy_start(ScriptedPolicy *policy, const Sim *sim);
void scripted_policy_update(ScriptedPolicy *policy, Sim *sim);

int scripted_policy_is_done(const ScriptedPolicy *policy);
const char *scripted_policy_state_name(ScriptedPolicyState state);

#endif
