#include "sim.h" 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// (Helper): Reset the wrapper so cleanup is always safe to call.
static void sim_clear (Sim *sim) {
    sim->mjModel = NULL;
    sim->mjData = NULL;
}

// (Helper): Print MuJoCo's XML loading errors.
static void print_load_error(const char *scene_path, const char *error) {
    assert(scene_path != NULL && error != NULL);
    fprintf(stderr, "ERROR: Failed to load MuJoCo's scene: %s\n", scene_path);
    if (error[0] != '\0') {
        fprintf(stderr, "ERROR: [MuJoCo] %s\n", error);
    }
}

void sim_load(Sim *sim, const char *scene_path) {
    assert(sim != NULL && scene_path != NULL);
    sim_clear(sim);

    char error[1024];
    error[0] = '\0';

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

int sim_find_site_id(const Sim *sim, const char *site_name) {
    assert(sim != NULL && site_name != NULL);
    assert(sim->model != NULL);

    int site_id = mj_name2id(sim->model, mjOBJ_SITE, site_name);
    if (site_id < 0) {
        fprintf(stderr, "ERROR: Failed to find integer ID for %s\n", site_name);
    }
    return site_id;
}

void sim_get_site_pos(const Sim *sim, const char *site_name, double out[3]) {
    assert(sim != NULL && site_name != NULL && out != NULL);
    assert(sim->data != NULL);
    int site_id = sim_find_site_id(sim, site_name);
    out[0] = sim->data->site_xpos[3 * site_id + 0];
    out[1] = sim->data->site_xpos[3 * site_id + 1];
    out[2] = sim->data->site_xpos[3 * site_id + 2];
}
