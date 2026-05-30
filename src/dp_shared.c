#include "dp_shared.h"

void add32flags(uint32_t rn_value, uint32_t target, uint32_t operand2) {
    update_zero_flag(target);
    update_negative_flag32(target);
    pState.c = (uint32_t)operand2 > ~(uint32_t)rn_value;
    pState.v = sign32(rn_value) == 0 && sign32(target) == 1;
}

void add64flags(uint64_t rn_value, uint64_t target, uint64_t operand2) {
    update_zero_flag(target);
    update_negative_flag64(target);
    pState.c = operand2 > ~rn_value;
    pState.v = sign64(rn_value) == 0 && sign64(target) == 1;
}

void sub32flags(uint32_t rn_value, uint32_t target, uint32_t operand2) {
    update_zero_flag(target);
    update_negative_flag32(target);
    pState.c = (uint32_t)operand2 <= (uint32_t)(rn_value);
    pState.v = (sign32(rn_value) ^ sign32(operand2)) && (sign32(target) != sign32(rn_value));
}

void sub64flags(uint64_t rn_value, uint64_t target, uint64_t operand2) {
    update_zero_flag(target);
    update_negative_flag64(target);
    pState.c = operand2 <= rn_value;
    pState.v = (sign64(rn_value) ^ sign64(operand2)) && (sign64(target) != sign64(rn_value));
}

void write_dp_result(uint32_t rd, uint32_t sf, uint64_t entry) {
    if (rd == ZERO_REGISTER_INDEX) {
        return;
    }
    if (sf == 0) {
        registers[rd] = (uint32_t)(registers[rd] & DP_CLEAR_LOWER_32_MASK)
                      | (entry & UINT32_MAX);
    } else {
        registers[rd] = entry;
    }
}