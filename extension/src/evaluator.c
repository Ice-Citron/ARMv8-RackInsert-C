#include <stdio.h>
#include <math.h>

void eval_compute_geometry(const EvalGeometry *geom, TrialScore *score) {
    double dx = geom->plug_tip[0] - geom->socket_mouth[0];
    double dy = geom->plug_tip[1] - geom->socket_mouth[1];
    double dz = geom->plug_tip[2] - geom->socket_mouth[2];

    score->plug_port_length = sqrt(dx*dx + dy*dy + dz*dz);
}

