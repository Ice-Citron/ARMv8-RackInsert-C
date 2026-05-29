#include "branch.h"

void unconditional_branch(uint32_t simm26) {
    long long offset = get_signed_value_from_bits(simm26 << 2);  // simm26 * 4
    pc += offset;
}

void register_branch(uint32_t xn) {
    if (0 <= xn || xn <= 30) {
        pc = registers[xn];
    }
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
            break;
    }
}

void conditional_branch(uint32_t simm19, uint32_t cond) {
    long long offset = get_signed_value_from_bits(simm19 << 2);  // simm19 * 4
    bool condition = switch (cond) {
        

    }
}

bool execute_branch(uint32_t instr) {
    if (extract_bits(31, 26, instr) == 0x05) {
        uint32_t simm26 = extract_bits(25, 0, instr);
        unconditional_branch(simm26);
    } else if (extract_bits(31, 10, instr) == 0x3587c0) {
        uint32_t xn = extract_bits(9, 5, instr);
        register_branch(xn);
    } else if (extract_bits(31, 24, instr) == 0x54) {
        uint32_t simm19 = extract_bits(23, 5, instr);
        uint32_t cond = extract_bits(3, 0, instr);
        conditional_branch(simm19, cond);
    } else {
        fprintf(stderr, "ERROR: Unknown type of branch instruction.\n");
    }
    
    return false;
}