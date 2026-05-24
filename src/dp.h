#include "emulate.h"

void add32flags (int32_t regindex, int32_t target, state* state, int32_t operand2) {
    if (target == 0) {
        (*state).z = true;
    }
    if (sign32(target) == 1) {
        (*state).n = true;
    }
    if (operand2 > ~get32from64(registers[regindex])) {
        (*state).c = true;
    }
    if (sign32(registers[regindex]) == 0 && sign32(target) == 1) {
        (*state).v = true;
    }
}

void add64flags (int32_t regindex, int64_t target, state* state, int64_t operand2) {
    if (target == 0) {
        (*state).z = true;
    }
    if (sign64(target) == 1) {
        (*state).n = true;
    }
    if (operand2 > ~registers[regindex]) {
        (*state).c = true;
    }
    if (sign64(registers[regindex]) == 0 && sign64(target) == 1) {
        (*state).v = true;
    }
}

void sub32flags (int32_t regindex, int32_t target, state* state, int32_t operand2) {
    if (target == 0) {
        (*state).z = true;
    }
    if (sign32(target) == 0) {
        (*state).n = true;
    }
    if (operand2 > get32from64(registers[regindex])) {
        (*state).c = true;
    }
    if (sign32(registers[regindex]) == 1 && sign32(target) == 0) {
        (*state).v = true;
    }
}

void sub64flags (int32_t regindex, int64_t target, state* state, int64_t operand2) {
    if (target == 0) {
        (*state).z = true;
    }
    if (sign64(target) == 0) {
        (*state).n = true;
    }
    if (operand2 > registers[regindex]) {
        (*state).c = true;
    }
    if (sign64(registers[regindex]) == 1 && sign64(target) == 0) {
        (*state).v = true;
    }
}