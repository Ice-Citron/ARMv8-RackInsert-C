#include "dp_shared.h"

#define SF_REG_HI  31
#define SF_REG_LO  31
#define OPC_REG_HI 30
#define OPC_REG_LO 29
#define M_REG_HI   28
#define M_REG_LO   28
#define RD_REG_HI  4
#define RD_REG_LO  0
#define RN_REG_HI  9
#define RN_REG_LO  5
#define RM_REG_HI  20
#define RM_REG_LO  16

#define M_REG_MULTIPLY 1
#define RA_REG_MULT_HI 14
#define RA_REG_MULT_LO 10
#define X_REG_MULT_HI  15
#define X_REG_MULT_LO  15
#define X_REG_MADD     0

#define OPR0_REG_HI       24
#define OPR0_REG_LO       24
#define OPR0_REG_ARITH    1
#define SHIFT_REG_HI      23
#define SHIFT_REG_LO      22
#define SHIFT_DIST_REG_HI 15
#define SHIFT_DIST_REG_LO 10
#define N_REG_HI          21
#define N_REG_LO          21

#define SHIFT_TYPE_LSL 0
#define SHIFT_TYPE_LSR 1
#define SHIFT_TYPE_ASR 2
#define SHIFT_TYPE_ROR 3

#define OPC_REG_AND        0
#define OPC_REG_ORR        1
#define OPC_REG_EOR        2
#define OPC_REG_ANDS_BICS  3

//helper to handle shifts and right rotate
static uint64_t shift_operand(uint64_t operand, uint32_t shift_type,
                              uint32_t shift_dist, uint32_t sf) {
    uint64_t mask = mask_from_sf(sf);
    uint32_t width = (sf == 0) ? 32 : 64;
    operand &= mask;

    if (shift_dist == 0) {
        return operand;
    } else if (shift_type == SHIFT_TYPE_LSL) {
        return (operand << shift_dist) & mask;
    } else if (shift_type == SHIFT_TYPE_LSR) {
        return operand >> shift_dist;
    } else if (shift_type == SHIFT_TYPE_ASR) {
        if (sf == 0) {
            return (uint32_t)((int32_t)operand >> shift_dist);
        } else {
            return (uint64_t)((int64_t)operand >> shift_dist);
        }
    } else if (shift_type == SHIFT_TYPE_ROR) {
        shift_dist %= width;
        if (shift_dist == 0) {
            return operand;
        }
        return ((operand >> shift_dist) | (operand << (width - shift_dist))) 
                                        & mask;
    } else {
        fprintf(stderr, "ERROR: Invalid shift type/dist detected.");
        exit(EXIT_FAILURE);
    }
}

//setting flags during register operations
static void set_logical_flags(uint64_t entry, uint32_t sf) {
    uint64_t masked = entry & mask_from_sf(sf);
    update_negative_flag(masked, sf);
    update_zero_flag(masked);
    pState.c = false;
    pState.v = false;
}

//helper to handle multiplication
static uint64_t compute_multiply_result(uint32_t instr, uint64_t rn, 
                                        uint64_t rm, uint64_t mask) {
    uint64_t entry = 0;
    uint32_t ra_index = extract_bits(RA_REG_MULT_HI,
                                     RA_REG_MULT_LO, instr);
    uint32_t x_mult   = extract_bits(X_REG_MULT_HI,
                                     X_REG_MULT_LO, instr);
    uint64_t ra_mult = read_dp_register(ra_index) & mask;
    uint64_t product = (rn * rm) & mask;

    if (x_mult == X_REG_MADD) {
        entry = (ra_mult + product) & mask;
    } else {
        entry = (ra_mult - product) & mask;
    }
    return entry;
}

//helper to handle bitwise logical operations
static uint64_t compute_logical_result(uint32_t instr, uint32_t opcode, 
                                       uint64_t rn, uint64_t operand2, 
                                       uint32_t sf) {
    uint64_t entry = 0;
    uint64_t mask = mask_from_sf(sf);
    if (extract_bits(N_REG_HI, N_REG_LO, instr) == 1) {
        operand2 = ~operand2 & mask;
    }
    switch (opcode) {
        case OPC_REG_AND:
            entry = rn & operand2;
            break;
        case OPC_REG_ORR:
            entry = rn | operand2;
            break;
        case OPC_REG_EOR:
            entry = rn ^ operand2;
            break;
        case OPC_REG_ANDS_BICS:
            entry = rn & operand2;
            clear_pstate_flags();
            set_logical_flags(entry, sf);
            break;
        default:
            fprintf(stderr, "ERROR: Unrecognised OPC type for Data Processing "
                            "instruction (Register).\n");
            exit(EXIT_FAILURE);
    }
    return entry;
}

//overall handling of register data processing instructions
void dpreg(uint32_t instr) {
    uint32_t sf       = extract_bits(SF_REG_HI , SF_REG_LO , instr);
    uint32_t opcode   = extract_bits(OPC_REG_HI, OPC_REG_LO, instr);
    uint32_t m_bit    = extract_bits(M_REG_HI  , M_REG_LO  , instr);
    uint32_t rd_index = extract_bits(RD_REG_HI , RD_REG_LO , instr);
    uint32_t rn_index = extract_bits(RN_REG_HI , RN_REG_LO , instr);
    uint32_t rm_index = extract_bits(RM_REG_HI , RM_REG_LO , instr);
    uint64_t mask = mask_from_sf(sf);
    uint64_t rn = read_dp_register(rn_index) & mask;
    uint64_t rm = read_dp_register(rm_index) & mask;
    uint64_t entry = 0;

    if (m_bit == M_REG_MULTIPLY) {
        entry = compute_multiply_result(instr, rn, rm, mask);
        write_dp_result(rd_index, sf, entry);
    } else {
        uint32_t opr0 = extract_bits(OPR0_REG_HI, OPR0_REG_LO, instr);
        uint32_t shift_type = extract_bits(SHIFT_REG_HI, SHIFT_REG_LO, instr);
        uint32_t shift_dist = extract_bits(SHIFT_DIST_REG_HI, SHIFT_DIST_REG_LO,
                                           instr);
        uint64_t operand2 = shift_operand(rm, shift_type, shift_dist, sf);

        if (shift_type == SHIFT_TYPE_ROR && opr0 == OPR0_REG_ARITH) {
            fprintf(stderr, "ERROR: Invalid rotate right with arithmetic op\n");
            exit(EXIT_FAILURE);
        }
        if (opr0 == OPR0_REG_ARITH) {
            entry = compute_add_sub_result(opcode, rn, operand2, mask, sf);
        } else {
            entry = compute_logical_result(instr, opcode, rn, operand2, sf);
        }
        write_dp_result(rd_index, sf, entry);
    }
}
