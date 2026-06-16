#ifndef EXTENSION_SIM_H
#define EXTENSION_SIM_H

#include <mujoco/mujoco.h>

typedef struct {
    mjModel *model;
    mjData  *data;
} Sim;

void sim_load(Sim *sim, const char *scene_path);
void sim_free(Sim *sim);
void sim_forward(Sim *sim);

int sim_find_site_id(const Sim *sim, const char *site_name);
void sim_get_site_pos(const Sim *sim, const char *site_name, double out[3]);

void sim_step(Sim *sim);
void sim_step_seconds(Sim *sim, double seconds);
void sim_set_ctrl(Sim *sim, int actuator_id, double value);
int  sim_find_actuator_id(const Sim *sim, const char *actuator_name);

#endif
