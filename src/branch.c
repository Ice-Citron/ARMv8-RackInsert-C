#include "branch.h"

static bool unconditional_branch(long long simm26) {
    pc = (uint64_t)((long long)pc + simm26);
    return true;
}

static bool register_branch(uint32_t xn) {
    if (xn == ZERO_REGISTER_INDEX) {
        fprintf(stderr, "ERROR: Branching with zero-register is invalid.\n");
        exit(EXIT_FAILURE);
    }
    pc = registers[xn];
    return true;
}

static bool condition_holds(uint32_t cond) {
    switch (cond) {
        case EQ_COND:
            return pState.z;
        case NE_COND:
            return !pState.z;
        case GE_COND:
            return pState.n == pState.v;
        case LT_COND:
            return pState.n != pState.v;
        case GT_COND:
            return !pState.z && pState.n == pState.v;
        case LE_COND:
            return !(!pState.z && pState.n == pState.v);
        case AL_COND:
            return true;
        default:
            fprintf(stderr, "ERROR: Unrecognised branch-instruction "
                            "condition.\n");
            exit(EXIT_FAILURE);
    }
}

static bool conditional_branch(long long simm19, uint32_t cond) {
    bool condition = condition_holds(cond);
    if (condition) {
        pc = (uint64_t)((long long)pc + simm19);
        return true;
    }
    return false;
}

bool execute_branch(uint32_t instr) {
    if (extract_bits(BR_UNCOND_HI, BR_UNCOND_LO, instr) == BR_TYPE_UNCOND) {
        long long simm26 = get_signed_value_from_bits(BR_SIMM26_HI, 
                                    BR_SIMM26_LO, instr) << BR_OFFSET_SHIFT;
        return unconditional_branch(simm26);
    } else if (extract_bits(BR_REG_HI, BR_REG_LO, instr) == BR_TYPE_REG) {
        uint32_t xn = extract_bits(BR_XN_HI, BR_XN_LO, instr);
        return register_branch(xn);
    } else if (extract_bits(BR_COND_HI, BR_COND_LO, instr) == BR_TYPE_COND) {
        long long simm19 = get_signed_value_from_bits(BR_SIMM19_HI, 
                                    BR_SIMM19_LO, instr) << BR_OFFSET_SHIFT;
        uint32_t cond = extract_bits(BR_COND_CODE_HI, BR_COND_CODE_LO, instr);
        return conditional_branch(simm19, cond);
    } else {
        fprintf(stderr, "ERROR: Unknown type of branch instruction.\n");
        exit(EXIT_FAILURE);
    }    
}
