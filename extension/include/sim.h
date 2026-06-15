#ifndef EXTENSION_SIM_H
#define EXTENSION_SIM_H

typedef struct {
    mjModel *model;
    mjData  *data;
} Sim;

int sim_load(Sim *sim, const char *scene_path);
void sim_free(Sim *sim);
void sim_forward(Sim *sim);

int sim_find_site_id(const Sim *sim, const char *site_name);
void sim_get_site_pos(const Sim *sim, const char *site_name, double out[3]);

#endif
