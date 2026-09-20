#ifndef EXTENSION_UTILS_H
#define EXTENSION_UTILS_H

#include <assert.h>
#include <stdlib.h>

#define ROBOT_ACTUATOR_JOINT_COUNT (7)

static const char *ACTUATOR_NAMES[ROBOT_ACTUATOR_JOINT_COUNT] = {
    "shoulder_pan_joint_motor",
    "shoulder_lift_joint_motor",
    "elbow_joint_motor",
    "wrist_1_joint_motor",
    "wrist_2_joint_motor",
    "wrist_3_joint_motor",
    "gripper/left_finger_joint_motor",
};

static const char *JOINT_NAMES[ROBOT_ACTUATOR_JOINT_COUNT] = {
    "shoulder_pan_joint",
    "shoulder_lift_joint",
    "elbow_joint",
    "wrist_1_joint",
    "wrist_2_joint",
    "wrist_3_joint",
    "gripper/left_finger_joint",
};

static const double HOME_QPOS[ROBOT_ACTUATOR_JOINT_COUNT] = {
    -0.1597,
    -1.3542,
    -1.6648,
    -1.6933,
    1.5710,
    1.4110,
    0.00655,
};

static inline double clamp(double x, double lo, double hi) {
    assert(lo <= hi);
    if (x < lo) return lo; 
    if (x > hi) return hi;
    return x;
}

static int parse_int_arg(const char *text, int *out) {
    assert(text != NULL && out != NULL);

    char *end = NULL;
    long value = strtol(text, &end, 10);

    if (end == text || *end != '\0' || value <= 0) {
        return 0;
    }

    *out = (int)value;
    return 1;
}

#endif