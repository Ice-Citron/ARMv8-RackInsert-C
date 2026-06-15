#include <sim.h>

#include <mujoco/mujoco.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// (Helper): Reset the wrapper so cleanup is always safe to call.
static void sim_clear (Sim *sim) {
    sim->mjModel = NULL;
    sim->mjData = NULL;
}

// 



void sim_free(Sim *sim) {
    if (sim == NULL)        { return; }
    if (sim->data != NULL)  { mj_deleteData(sim->data); }
    if (sim->model != NULL) { mj_deleteModel(sim->model); }
    sim_clear(sim);
}

mjModel *load_model_or_die(const char *path) {
    char error[1024];
    
    mjModel *model = mj_loadXML(path, NULL, error, sizeof(error));
    if (model == NULL) {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    return model;
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
    int site_id = sim_find_site_id(sim, site_name);
    out[0] = sim->data->site_xpos[3 * site_id + 0];
    out[1] = sim->data->site_xpos[3 * site_id + 1];
    out[2] = sim->data->site_xpos[3 * site_id + 2];
}

int main(int argc, char **argv) {
    mjModel *model = mjModel(argv[1]);
    mjData *data = mj_makeData(model);
    if (data == NULL) {
        fprintf(stderr, "ERROR: Out of memory for mj_makeData.\n");
        mj_deleteModel(model);
        exit(EXIT_FAILURE);
    }

    // Advance the simulation 1000 steps
    for (int i = 0; i < 1000; i++) {
        mj_step(model, data);
    }

    // Clean up memory (data first, then model)
    mj_deleteData(data);
    mj_deleteModel(model);
    return 0;
}


