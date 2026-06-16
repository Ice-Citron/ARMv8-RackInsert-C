#include "logging.h"

#include <stdio.h>

// Console logs 3D-vector information
void print_vec3(const char *label, const double v[3]) {
    printf("%-14s = %.6f %.6f %.6f\n", label, v[0], v[1], v[2]);
}

// Once evaluation run completes, the final state and scores are printed
void print_eval_report(const char *scene_path, const EvalGeometry *geom,
                       const TrialScore *score, double socket_depth) {
    printf("\n\n");
    printf("scene = %s\n", scene_path);
    print_vec3("plug_tip",      geom->plug_tip);
    print_vec3("socket_mouth",  geom->socket_mouth);
    print_vec3("socket_bottom", geom->socket_bottom);

    printf("\n");
    printf("socket_depth    = %.6f\n", socket_depth);
    printf("plug_port_dist  = %.6f\n", score->plug_port_distance);
    printf("lateral_error   = %.6f\n", score->lateral_error);
    printf("axial_depth     = %.6f\n", score->axial_depth);

    printf("\n");
    printf("full_insertion  = %d\n", score->full_insertion);
    printf("partial_insert  = %d\n", score->partial_insertion);

    printf("\n");
    printf("SCORES:\n");
    printf("    tier1 = %.2f\n", score->tier1);
    printf("    tier2 = %.2f\n", score->tier2);
    printf("    tier3 = %.2f\n", score->tier3);
    printf("    total = %.2f\n", score->total);
}

void print_usage_guide(const char *program_name) {
    fprintf(stderr, "ERROR: See proper usage template below:\n");
    fprintf(stderr, "   %s\n", program_name);
    fprintf(stderr, "   %s --scene <path-to-mjcf-scene>\n", program_name);
}