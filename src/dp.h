#include "emulate.h"

static uint32_t get32from64(uint64_t target) {
    return (uint32_t) target;
}

static uint64_t flag_register_value(uint32_t regindex) {
    return regindex == 31 ? zeroRegister : registers[regindex];
}

void add32flags(uint32_t regindex, uint64_t target, state* state, uint64_t operand2) {
    if (target == 0) {
        (*state).z = true;
    }
    if (sign32(target) == 1) {
        (*state).n = true;
    }
    if ((uint32_t) operand2 > ~get32from64(flag_register_value(regindex))) {
        (*state).c = true;
    }
    if (sign32(flag_register_value(regindex)) == 0 && sign32(target) == 1) {
        (*state).v = true;
    }
}

void add64flags(uint32_t regindex, uint64_t target, state* state, uint64_t operand2) {
    if (target == 0) {
        (*state).z = true;
    }
    if (sign64(target) == 1) {
        (*state).n = true;
    }
    if ((uint64_t) operand2 > ~flag_register_value(regindex)) {
        (*state).c = true;
    }
    if (sign64(flag_register_value(regindex)) == 0 && sign64(target) == 1) {
        (*state).v = true;
    }
}

void sub32flags(uint32_t regindex, uint64_t target, state* state, uint64_t operand2) {
    if (target == 0) {
        (*state).z = true;
    }
    if (sign32(target) == 1) {
        (*state).n = true;
    }
    if ((uint32_t) operand2 > get32from64(flag_register_value(regindex))) {
        (*state).c = true;
    }
    if (sign32(flag_register_value(regindex)) == 1 && sign32(target) == 0) {
        (*state).v = true;
    }
}

void sub64flags(uint32_t regindex, uint64_t target, state* state, uint64_t operand2) {
    if (target == 0) {
        (*state).z = true;
    }
    if (sign64(target) == 1) {
        (*state).n = true;
    }
    if ((uint64_t) operand2 > flag_register_value(regindex)) {
        (*state).c = true;
    }
    if (sign64(flag_register_value(regindex)) == 1 && sign64(target) == 0) {
        (*state).v = true;
    }
}
