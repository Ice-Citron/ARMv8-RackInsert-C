#ifndef EXTENSION_SIM_H
#define EXTENSION_SIM_H

#include <mujoco/mujoco.h>

typedef struct {
    mjModel *model;
    mjData  *data;
} Sim;

void sim_load(Sim *sim, const char *scene_path);
void sim_reset(Sim *sim);
void sim_free(Sim *sim);

void sim_forward(Sim *sim);
void sim_step(Sim *sim);
void sim_step_seconds(Sim *sim, double seconds);

// MuJoCo `site` related functions
int  sim_find_site_id(const Sim *sim, const char *site_name);
void sim_get_site_pos_by_id(const Sim *sim, int site_id, double out[3]);
void sim_get_site_pos(const Sim *sim, const char *site_name, double out[3]);

// Robot-arm's `joint` related functions
int  sim_find_joint_id(const Sim *sim, const char *joint_name);
double sim_get_joint_qpos(const Sim *sim, const char *joint_name);

// Robot-arm's `actuator` related functions
int  sim_find_actuator_id(const Sim *sim, const char *actuator_name);
void sim_set_ctrl(Sim *sim, int actuator_id, double value);
void sim_set_actuator_by_name(Sim *sim, const char *actuator_name, 
                              double value);

#endif
