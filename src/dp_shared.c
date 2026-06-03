#include "dp_shared.h"

#define DP_CLEAR_LOWER_32_MASK (~(uint64_t) UINT32_MAX)
#define DP_OPC_ADD_NOFLAG     0
#define DP_OPC_ADD_SETFLAG    1
#define DP_OPC_SUB_NOFLAG     2
#define DP_OPC_SUB_SETFLAG    3

static inline void set_add_flags(uint64_t rn_value, uint64_t target, 
                                 uint64_t operand2, uint32_t sf) {
    update_zero_flag(target);
    update_negative_flag(target, sf);
    if (sf == 0) {
        pState.c = (uint32_t)operand2 > ~(uint32_t)rn_value;
        pState.v = (sign32(rn_value) == sign32(operand2)) && 
            (sign32(target) != sign32(rn_value));
    } else {
        pState.c = operand2 > ~rn_value;
        pState.v = (sign64(rn_value) == sign64(operand2)) && 
            (sign64(target) != sign64(rn_value));
    }
}

static inline void set_sub_flags(uint64_t rn_value, uint64_t target, 
                                 uint64_t operand2, uint32_t sf) {
    update_zero_flag(target);
    update_negative_flag(target, sf);
    if (sf == 0) {
        pState.c = (uint32_t)operand2 <= (uint32_t)(rn_value);
        pState.v = (sign32(rn_value) ^ sign32(operand2)) && 
            (sign32(target) != sign32(rn_value));
    } else {
        pState.c = operand2 <= rn_value;
        pState.v = (sign64(rn_value) ^ sign64(operand2)) && 
            (sign64(target) != sign64(rn_value));    
    }
}

void write_dp_result(uint32_t rd, uint32_t sf, uint64_t entry) {
    if (rd == ZERO_REGISTER_INDEX) {
        return;
    }
    if (sf == 0) {
        registers[rd] = (uint64_t)(uint32_t)entry;
    } else {
        registers[rd] = entry;
    }
}

uint64_t compute_add_sub_result(uint32_t opc, uint64_t rn, uint64_t operand2, 
                                uint64_t mask, uint32_t sf) {
    uint64_t entry;
    switch (opc) {
        case DP_OPC_ADD_NOFLAG:
            return (rn + operand2) & mask;
        case DP_OPC_ADD_SETFLAG:
            entry = (rn + operand2) & mask; 
            set_add_flags(rn, entry, operand2, sf);
            return entry;
        case DP_OPC_SUB_NOFLAG:
            return (rn - operand2) & mask;
        case DP_OPC_SUB_SETFLAG:
            entry = (rn - operand2) & mask;
            set_sub_flags(rn, entry, operand2, sf);
            return entry;
        default:
            fprintf(stderr, "ERROR: Unrecognised opcode for data processing\n");
            exit(EXIT_FAILURE);
    }
}
