#include "dp_shared.h"

void add32flags(uint32_t regindex, uint64_t target, state* state, uint64_t operand2) {
    update_zero_flag(state, target);
    update_negative_flag32(state, target);
    state->c = (uint32_t) operand2 > ~(uint64_t)(flag_register_value(regindex));
    state->v = sign32(flag_register_value(regindex)) == 0 && sign32(target) == 1;
}

void add64flags(uint32_t regindex, uint64_t target, state* state, uint64_t operand2) {
    update_zero_flag(state, target);
    update_negative_flag64(state, target);
    state->c = (uint64_t) operand2 > ~flag_register_value(regindex);
    state->v = sign64(flag_register_value(regindex)) == 0 && sign64(target) == 1;
}

void sub32flags(uint32_t regindex, uint64_t target, state* state, uint64_t operand2) {
    update_zero_flag(state, target);
    update_negative_flag32(state, target);
    state->c = (uint32_t) operand2 > (uint64_t)(flag_register_value(regindex));
    state->v = sign32(flag_register_value(regindex)) == 1 && sign32(target) == 0;
}

void sub64flags(uint32_t regindex, uint64_t target, state* state, uint64_t operand2) {
    update_zero_flag(state, target);
    update_negative_flag64(state, target);
    state->c = (uint64_t) operand2 > flag_register_value(regindex);
    state->v = sign64(flag_register_value(regindex)) == 1 && sign64(target) == 0;
}

void write_dp_result(uint32_t rd, uint32_t sf, uint64_t entry) {
    if (rd == ZERO_REGISTER_INDEX) {
        return;
    }
    if (sf == 0) {
        registers[rd] = (registers[rd] & DP_CLEAR_LOWER_32_MASK)
                      | (entry & UINT32_MAX);
    } else {
        registers[rd] = entry;
    }
}