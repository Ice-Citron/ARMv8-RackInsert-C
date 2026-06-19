#ifndef EXTENSION_LOGGING_H
#define EXTENSION_LOGGING_H

#include "evaluator.h"
#include "scripted_policy.h"

#include <stdio.h>

void print_vec3(const char *label, const double v[3]);
void print_eval_report(const char *scene_path, const EvalGeometry *geom,
                       const TrialScore *score, double socket_depth);
void print_usage_guide(const char *program_name);
void print_trial_summary(const char *title, const TrialScore *initial, 
                         const TrialScore *final, double plug_motion, 
                         double duration);

// Benchmark module. Acts as simulation's telemetry logger, converting live
// MuJoCo Physics data into readable CS file.
void write_trace_header(FILE *trace, const Sim *sim);
void write_trace_row(FILE *trace, const Sim *sim, const ScriptedPolicy *policy);

#endif
