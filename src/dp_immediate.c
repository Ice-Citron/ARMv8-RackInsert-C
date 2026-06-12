#include "dp_shared.h"

#define SF_IMM_HI  31
#define SF_IMM_LO  31
#define OPC_IMM_HI 30
#define OPC_IMM_LO 29
#define OPI_IMM_HI 25
#define OPI_IMM_LO 23
#define RD_IMM_HI  4
#define RD_IMM_LO  0

#define OPI_IMM_ARITH   2
#define RN_IMM_ARITH_HI 9
#define RN_IMM_ARITH_LO 5
#define SH_IMM_ARITH_HI 22
#define SH_IMM_ARITH_LO 22
#define IMM12_IMM_HI    21
#define IMM12_IMM_LO    10
#define IMM_ARITH_SHIFT_AMOUNT 12

#define OPI_IMM_WIDE_MOVE 5
#define SH_IMM_WM_HI      22
#define SH_IMM_WM_LO      21
#define IMM16_IMM_WM_HI   20
#define IMM16_IMM_WM_LO   5

#define IMM_WIDE_MOVE_SHIFT_FACTOR 16
#define OPC_IMM_MOVN 0
#define OPC_IMM_MOVZ 2
#define OPC_IMM_MOVK 3

/*
 * Computes the result of an arithmetic immediate instruction.
 * Example: ADD with Rn = 10 and immediate 5 returns 15.
 */
static uint64_t compute_arithmetic_result(uint32_t instr, uint32_t opcode, 
                                          uint64_t mask, uint32_t sf) {
    uint32_t rn_arith    = extract_bits(RN_IMM_ARITH_HI, RN_IMM_ARITH_LO, 
                                        instr);
    uint32_t shift_arith = extract_bits(SH_IMM_ARITH_HI, SH_IMM_ARITH_LO, 
                                        instr);
    uint64_t operand2    = extract_bits(IMM12_IMM_HI, IMM12_IMM_LO, instr);
    uint64_t rn_value    = read_dp_register(rn_arith) & mask;
    
    if (shift_arith) {
        operand2 <<= IMM_ARITH_SHIFT_AMOUNT;
    }
    operand2 &= mask;
    return compute_add_sub_result(opcode, rn_value, operand2, mask, sf);
}

/*
 * Computes the result of a wide move instruction: MOVN, MOVZ or MOVK.
 * Example: MOVZ with imm16 = 0xabcd and shift 0 returns 0xabcd.
 */
static uint64_t compute_wide_move_result(uint32_t instr, uint32_t opcode, 
                                         uint32_t rd, uint64_t mask) {
    uint32_t hw_wm    = extract_bits(SH_IMM_WM_HI, SH_IMM_WM_LO, instr);
    uint64_t imm16_wm = extract_bits(IMM16_IMM_WM_HI, IMM16_IMM_WM_LO, instr);
    uint32_t shift    = IMM_WIDE_MOVE_SHIFT_FACTOR * hw_wm;
    uint64_t shifted  = (imm16_wm << shift) & mask;
    switch (opcode) {
        case OPC_IMM_MOVN:
            return (~shifted) & mask;
        case OPC_IMM_MOVZ: 
            return shifted;
        case OPC_IMM_MOVK: {
            uint64_t old = read_dp_register(rd) & mask;
            uint64_t clear_mask = ~((uint64_t)DP_16BIT_MASK << shift) & mask;
            return (old & clear_mask) | shifted;
        }
        default:
            fprintf(stderr, "invalid opcode for dp imm widemove\n");
            exit(EXIT_FAILURE);
    }
}

/*
 * Decodes and executes data-processing immediate instructions.
 * Example: dispatches ADD/SUB immediate to compute_arithmetic_result
 * and MOVZ/MOVK/MOVN to compute_wide_move_result.
 */
void dpimm(uint32_t instr) {
    uint32_t sf     = extract_bits(SF_IMM_HI , SF_IMM_LO , instr);
    uint32_t opcode = extract_bits(OPC_IMM_HI, OPC_IMM_LO, instr);
    uint32_t opi    = extract_bits(OPI_IMM_HI, OPI_IMM_LO, instr);
    uint32_t rd     = extract_bits(RD_IMM_HI , RD_IMM_LO , instr);
    uint64_t mask   = mask_from_sf(sf);
    uint64_t entry = 0;
    if (opi == OPI_IMM_ARITH) {
        entry = compute_arithmetic_result(instr, opcode, mask, sf);
    } else if (opi == OPI_IMM_WIDE_MOVE) {
        entry = compute_wide_move_result(instr, opcode, rd, mask);
    } else {
        fprintf(stderr, "ERROR: Unrecognised opcode for data-processing "
                        "immediate instruction.\n");
        exit(EXIT_FAILURE);
    }
    write_dp_result(rd, sf, entry);
}
