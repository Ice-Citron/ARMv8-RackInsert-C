#include "branch.h"

void unconditional_branch(long long simm26) {
    pc = (uint64_t)((long long)pc + simm26);
    return true;
}

void register_branch(uint32_t xn) {
    if (xn == 31) {
        fprintf(stderr, "ERROR: Branching with zero-register is invalid.\n");
        return false;
    }
    pc = registers[xn];
    return true;
}

bool condition_holds(uint32_t cond) {
    switch (cond) {
        case 0x00: // EQ
            return pState.z;
            break;
        case 0x01: // NE
            return !pState.z;
            break;
        case 0x0a: // GE
            return pState.n == pState.v;
            break;
        case 0x0b: // LT
            return pState.n != pState.v;
            break;
        case 0x0c: // GT
            return !pState.z && pState.n == pState.v;
            break;
        case 0x0d: // LE
            return !(!pState.z && pState.n == pState.v);
            break;
        case 0x0e: // AL
            return true;
            break;
        default:
            fprintf(stderr, "ERROR: Unrecognised branch-instruction "
                            "condition.\n");
            return false;
    }
}

void conditional_branch(long long simm19, uint32_t cond) {
    bool condition = condition_holds(cond);
    if (condition) {
        pc = (uint64_t)((long long)pc + simm19);
        return true;
    }
    return false;
}

bool execute_branch(uint32_t instr) {
    if (extract_bits(31, 26, instr) == 0x05) {
        long long simm26 = get_signed_value_from_bits(25, 0, instr) << 2;
        return unconditional_branch(simm26);
    } else if (extract_bits(31, 10, instr) == 0x3587c0) {
        uint32_t xn = extract_bits(9, 5, instr);
        return register_branch(xn);
    } else if (extract_bits(31, 24, instr) == 0x54) {
        long long simm19 = get_signed_value_from_bits(23, 5, instr) << 2;
        uint32_t cond = extract_bits(3, 0, instr);
        return conditional_branch(simm19, cond);
    } else {
        fprintf(stderr, "ERROR: Unknown type of branch instruction.\n");
    }
    
    return false;
}