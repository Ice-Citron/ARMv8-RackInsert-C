#include "evaluator.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>

static void assert_close(double actual, double expected, double tol) {
    assert(fabs(actual - expected) <= tol);
}

static void test_geometry_halfway_inserted(void) {
    EvalGeometry geom = {
        .plug_tip      = {0.0, 0.0, 0.05},
        .socket_mouth  = {0.0, 0.0, 0.0},
        .socket_bottom = {0.0, 0.0, 0.10},
    };

    TrialScore score = {0};
    eval_compute_geometry(&geom, &score);

    assert_close(score.axial_depth, 0.05, 1e-9);
    assert_close(score.lateral_error, 0.0, 1e-9);
    assert_close(score.plug_port_distance, 0.05, 1e-9);
}

static void test_geometry_lateral_error(void) {
    Eval_Geometry geom = {
        .plug_tip      = {0.03, 0.0, 0.05},
        .socket_mouth  = {0.0, 0.0, 0.0},
        .socket_bottom = {0.0, 0.0, 0.10},
    };

    TrialScore score = {0};
    eval_compute_geometry(&geom, &score);

    assert_close(score.axial_depth, 0.05, 1e-9);
    assert_close(score.lateral_error, 0.03, 1e-9);
}
