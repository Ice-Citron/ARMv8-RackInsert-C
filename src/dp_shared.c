#include "dp_shared.h"

void add32flags(uint32_t rn_value, uint32_t target, uint32_t operand2) {
    update_zero_flag(target);
    update_negative_flag32(target);
    pState.c = (uint32_t)operand2 > ~(uint32_t)rn_value;
    pState.v = (sign32(rn_value) == sign32(operand2)) && (sign32(target) != sign32(rn_value));
}

void add64flags(uint64_t rn_value, uint64_t target, uint64_t operand2) {
    update_zero_flag(target);
    update_negative_flag64(target);
    pState.c = operand2 > ~rn_value;
    pState.v = (sign64(rn_value) == sign64(operand2)) && (sign64(target) != sign64(rn_value));
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

uint64_t compute_add_sub_result(uint32_t switcher, uint64_t rn, 
                                uint64_t operand2, uint64_t mask, uint32_t sf) {
        uint64_t entry;
        switch (switcher) {
            case DP_OPC_ADD_NOFLAG:
                return (rn + operand2) & mask;;
            case DP_OPC_ADD_SETFLAG:
                entry = (rn + operand2) & mask; 
                if (sf == 0) {
                    add32flags(rn, entry, operand2);
                } else {
                    add64flags(rn, entry, operand2);
                }
                return entry;
            case DP_OPC_SUB_NOFLAG:
                return (rn - operand2) & mask;
            case DP_OPC_SUB_SETFLAG:
                entry = (rn - operand2) & mask;
                if (sf == 0) {
                    sub32flags(rn, entry, operand2);
                } else {
                    sub64flags(rn, entry, operand2);
                }
                return entry;
            default:
                fprintf(stderr, "ERROR: Unrecognised opcode for Data"
                                " Processing.\n");
                exit(EXIT_FAILURE);
            }
    }