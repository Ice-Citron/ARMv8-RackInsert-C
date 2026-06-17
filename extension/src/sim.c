#include "sim.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// (Helper): Reset the wrapper so cleanup is always safe to call.
static void sim_clear (Sim *sim) {
    sim->model = NULL;
    sim->data  = NULL;
}

// (Helper): Print MuJoCo's XML loading errors.
static void print_load_error(const char *scene_path, const char *error) {
    assert(scene_path != NULL && error != NULL);
    fprintf(stderr, "ERROR: Failed to load MuJoCo's scene: %s\n", scene_path);
    if (error[0] != '\0') {
        fprintf(stderr, "ERROR: [MuJoCo] %s\n", error);
    }
}

// (Helper): Loads MuJoCo plugins, since C/MuJoCo doesn't auto-handle this.
static void load_mujoco_plugins(void) {
    static int plugins_loaded = 0;

    if (!plugins_loaded) {
        plugins_loaded = 1;
        mj_loadAllPluginLibraries(MUJOCO_PLUGIN_DIR, NULL);    
    }
}

void sim_load(Sim *sim, const char *scene_path) {
    assert(sim != NULL && scene_path != NULL);
    sim_clear(sim);

    char error[1024];
    error[0] = '\0';

    load_mujoco_plugins();
    sim->model = mj_loadXML(scene_path, NULL, error, sizeof(error));
    if (sim->model == NULL) {
        print_load_error(scene_path, error);
        exit(EXIT_FAILURE);
    }

    sim->data = mj_makeData(sim->model);
    if (sim->data == NULL) {
        fprintf(stderr, "ERROR: Failed to allocate mjData.\n");
        sim_free(sim);
        exit(EXIT_FAILURE);
    }

    // Refreshes ghost coordinates from last trial, recalculates all kinematics.
    mj_forward(sim->model, sim->data);
}

void sim_reset(Sim *sim) {
    assert(sim != NULL);
    assert(sim->data != NULL && sim->model != NULL);

    mj_resetData(sim->model, sim->data);
    mj_forward(sim->model, sim->data);   // Re-calcs physics whilst time frozen
}

void sim_free(Sim *sim) {
    if (sim == NULL)        { return; }
    if (sim->data != NULL)  { mj_deleteData(sim->data); }
    if (sim->model != NULL) { mj_deleteModel(sim->model); }
    sim_clear(sim);
}

void sim_forward(Sim *sim) {
    assert(sim != NULL);
    assert(sim->model != NULL && sim->data != NULL);
    mj_forward(sim->model, sim->data);
}

void sim_step(Sim *sim) {
    assert(sim != NULL);
    assert(sim->model != NULL && sim->data != NULL);
    
    mj_step(sim->model, sim->data);
}

void sim_step_seconds(Sim *sim, double seconds) {
    assert(sim != NULL);
    assert(sim->data != NULL);
    assert(seconds >= 0.0);

    double end_time = sim->data->time + seconds;
    while (sim->data->time < end_time) {
        sim_step(sim);
    }
}

int sim_find_site_id(const Sim *sim, const char *site_name) {
    assert(sim != NULL && site_name != NULL);
    assert(sim->model != NULL);

    int site_id = mj_name2id(sim->model, mjOBJ_SITE, site_name);
    if (site_id < 0) {
        fprintf(stderr, "ERROR: Failed to find integer ID for site %s\n", 
                site_name);
        exit(EXIT_FAILURE);
    }
    return site_id;
}

void sim_get_site_pos_by_id(const Sim *sim, int site_id, double out[3]) {
    assert(sim != NULL && out != NULL);
    assert(sim->data != NULL && sim->model != NULL);
    assert(site_id >= 0 && site_id < sim->model->nsite);

    out[0] = sim->data->site_xpos[3 * site_id + 0];
    out[1] = sim->data->site_xpos[3 * site_id + 1];
    out[2] = sim->data->site_xpos[3 * site_id + 2];
}

void sim_get_site_pos(const Sim *sim, const char *site_name, double out[3]) {
    assert(sim != NULL && site_name != NULL && out != NULL);
    assert(sim->data != NULL);
    
    int site_id = sim_find_site_id(sim, site_name);
    sim_get_site_pos_by_id(sim, site_id, out);
}


int sim_find_joint_id(const Sim *sim, const char *joint_name) {
    assert(sim != NULL && joint_name != NULL);
    assert(sim->model != NULL);

    int site_id = mj_name2id(sim->model, mjOBJ_JOINT, joint_name);
    if (site_id < 0) {
        fprintf(stderr, "ERROR: Failed to find integer ID for joint %s\n", 
                joint_name);
        exit(EXIT_FAILURE);
    }
    return site_id;
}

double sim_get_joint_qpos(const Sim *sim, const char *joint_name) {
    assert(sim != NULL && joint_name != NULL);
    assert(sim->data != NULL && sim->model != NULL);

    int joint_id = sim_find_joint_id(sim, joint_name);
    int qpos_addr = sim->model->jnt_qposadr[joint_id];
    return sim->data->qpos[qpos_addr];
}

// Finds integer ID of actuator of robotic arm based on `actuator_name`.
int sim_find_actuator_id(const Sim *sim, const char *actuator_name) {
    assert(sim != NULL && actuator_name != NULL);
    assert(sim->model != NULL);

    int actuator_id = mj_name2id(sim->model, mjOBJ_ACTUATOR, actuator_name);
    if (actuator_id < 0) {
        fprintf(stderr, "ERROR: Failed to find ID for %s.\n", actuator_name);
        exit(EXIT_FAILURE);
    }
    return actuator_id;
}

void sim_set_ctrl(Sim *sim, int actuator_id, double value) {
    assert(sim != NULL);
    assert(sim->data != NULL && sim->model != NULL);
    assert(actuator_id >= 0 && actuator_id < sim->model->nu);
    
    sim->data->ctrl[actuator_id] = value;   
}
