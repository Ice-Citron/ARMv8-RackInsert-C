#include <mujoco/mujoco.h>
#include <stdio.h>
#include <stdlib.h>


mjModel *load_model_or_die(const char *path) {
    char error[1024];
    
    mjModel *model = mj_loadXML(path, NULL, error, sizeof(error));
    if (model == NULL) {
        fprintf(stderr, "%s\n", error);
        exit(EXIT_FAILURE);
    }

    return model;
}

int lookup_int_id(const char *name) {
    int plug_tip_id = mj_name2id(model, mjOBJ_SITE, "plug_tip");
    if (plug_tip_id < 0) {  // Returns -1 if not found
        fprintf(stderr, "ERROR: Failed to find integer ID of plug tip.\n");
        exit(EXIT_FAILURE);
    }
    return plug_tip_id;
}

void get_site_pos(const mjData *data, int site_id, double out[3]) {
    out[0] = data->site_xpos[3 * site_id + 0];
    out[1] = data->site_xpos[3 * site_id + 1];
    out[2] = data->site_xpos[3 * site_id + 2];
}


void set_actuator(const mjModel *model, mjData *data, const char *name, 
                  double target) {
    int id = mj_name2id(model, mjOBJ_ACTUATOR, name);
    if (id < 0) {
        fprintf(stderr, "ERROR: Missing actuator %s\n", name);
        exit(EXIT_FAILURE);
    }

    data->ctrl[id] = target;
}

void step_for_seconds(const mjModel *model, mjData *data, double duration_s) {
    int steps = (int)(duration_s / model->opt.timestep);

    for (int i = 0; i < steps; i++) {
        mj_step(model, data);
    }
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


