#include "branch.h"
#include "bit_manipulation.h"

#define EQ_COND 0u
#define NE_COND 1u
#define GE_COND 10u
#define LT_COND 11u
#define GT_COND 12u
#define LE_COND 13u
#define AL_COND 14u

#define BR_TYPE_UNCOND  0x05
#define BR_TYPE_REG     0x3587c0
#define BR_TYPE_COND    0x54

#define BR_UNCOND_HI    31
#define BR_UNCOND_LO    26
#define BR_REG_HI       31
#define BR_REG_LO       10
#define BR_COND_HI      31
#define BR_COND_LO      24
#define BR_SIMM26_HI    25
#define BR_SIMM26_LO    0
#define BR_SIMM19_HI    23
#define BR_SIMM19_LO    5
#define BR_XN_HI        9
#define BR_XN_LO        5
#define BR_COND_CODE_HI 3
#define BR_COND_CODE_LO 0
#define BR_OFFSET_SHIFT 2

/*
 * (Helper) Executes an unconditional branch by adding a signed offset to PC.
 * Example: PC = 100 and offset = 8 updates PC to 108.
 */
static bool unconditional_branch(long long simm26) {
    pc = (uint64_t)((long long)pc + simm26);
    return true;
}

/*
 * (Helper) Executes a register branch by copying a register value into PC.
 * Example: if X5 contains 0x400, BR X5 sets PC to 0x400.
 */
static bool register_branch(uint32_t xn) {
    if (xn == ZERO_REGISTER_INDEX) {
        fprintf(stderr, "ERROR: Branching with zero-register is invalid.\n");
        exit(EXIT_FAILURE);
    }
    pc = registers[xn];
    return true;
}

/*
 * (Helper) Checks whether a branch condition holds using PSTATE flags.
 * Example: EQ returns true when the Z flag is set.
 */
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

/*
 * (Helper) Executes a conditional branch if its condition is true.
 * Example: B.EQ updates PC only when the Z flag is set.
 */
static bool conditional_branch(long long simm19, uint32_t cond) {
    bool condition = condition_holds(cond);
    if (condition) {
        pc = (uint64_t)((long long)pc + simm19);
        return true;
    }
    return false;
}

/*
 * Decodes and executes branch instructions.
 * Example: dispatches B, BR and B.cond based on the instruction encoding.
 */
bool execute_branch(uint32_t instr) {
    if (extract_bits(BR_UNCOND_HI, BR_UNCOND_LO, instr) == BR_TYPE_UNCOND) {
        long long simm26 = get_signed_value(BR_SIMM26_HI, BR_SIMM26_LO, instr) 
                                            << BR_OFFSET_SHIFT;
        return unconditional_branch(simm26);
    } else if (extract_bits(BR_REG_HI, BR_REG_LO, instr) == BR_TYPE_REG) {
        uint32_t xn = extract_bits(BR_XN_HI, BR_XN_LO, instr);
        return register_branch(xn);
    } else if (extract_bits(BR_COND_HI, BR_COND_LO, instr) == BR_TYPE_COND) {
        long long simm19 = get_signed_value(BR_SIMM19_HI, BR_SIMM19_LO, instr) 
                                            << BR_OFFSET_SHIFT;
        uint32_t cond = extract_bits(BR_COND_CODE_HI, BR_COND_CODE_LO, instr);
        return conditional_branch(simm19, cond);
    } else {
        fprintf(stderr, "ERROR: Unknown type of branch instruction.\n");
        exit(EXIT_FAILURE);
    }    
}
