#include "dp_immediate.h"

static uint64_t compute_arithmetic_result() {

}

static uint64_t compute_wide_move_result() {
    uint32_t hw_wm = extract_bits(SH_IMM_WM_HI, SH_IMM_WM_LO, instr);
    uint64_t imm16_wm = extract_bits(IMM16_IMM_WM_HI, IMM16_IMM_WM_LO, instr);
    uint32_t shift = IMM_WIDE_MOVE_SHIFT_FACTOR * hw_wm;
    uint64_t shifted = (imm16_wm << shift) & mask;
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

void dpimm(uint32_t instr) {
    uint32_t sf     = extract_bits(SF_IMM_HI , SF_IMM_LO , instr);
    uint32_t opcode = extract_bits(OPC_IMM_HI, OPC_IMM_LO, instr);
    uint32_t opi    = extract_bits(OPI_IMM_HI, OPI_IMM_LO, instr);
    uint32_t rd     = extract_bits(RD_IMM_HI , RD_IMM_LO , instr);
    uint64_t mask   = mask_from_sf(sf);
    uint64_t entry  = 0;

    if (opi == OPI_IMM_ARITH) {
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
        entry = compute_add_sub_result(opcode, rn_value, operand2, mask, sf);
    } else if (opi == OPI_IMM_WIDE_MOVE) {
        entry = compute 
        /*
        uint32_t hw_wm = extract_bits(SH_IMM_WM_HI, SH_IMM_WM_LO,
                                      instr);
        uint64_t imm16_wm = extract_bits(IMM16_IMM_WM_HI,
                                         IMM16_IMM_WM_LO, instr);
        uint32_t shift = IMM_WIDE_MOVE_SHIFT_FACTOR * hw_wm;
        uint64_t shifted = (imm16_wm << shift) & mask;
        switch (opcode) {
            case OPC_IMM_MOVN:
                entry = (~shifted) & mask;
                break;
            case OPC_IMM_MOVZ: 
                entry = shifted;
                break;
            case OPC_IMM_MOVK: {
                uint64_t old = read_dp_register(rd) & mask;
                uint64_t clear_mask = ~((uint64_t)DP_16BIT_MASK << shift) & mask;
                entry = (old & clear_mask) | shifted;
                break;
            }
            default:
                fprintf(stderr, "invalid opcode for dp imm widemove\n");
                exit(EXIT_FAILURE);
        }
        */ 
    }
    write_dp_result(rd, sf, entry);
}
