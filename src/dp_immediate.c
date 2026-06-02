#include "dp_immediate.h"

void dpimm(uint32_t instr) {
    uint32_t sf_dpimm  = extract_bits(SF_DPIMM_START, SF_DPIMM_END, instr);
    uint32_t opc_dpimm = extract_bits(OPC_DPIMM_START, OPC_DPIMM_END, instr);
    uint32_t opi_dpimm = extract_bits(OPI_DPIMM_START, OPI_DPIMM_END, instr);
    uint32_t rd_dpimm  = extract_bits(RD_DPIMM_START, RD_DPIMM_END, instr);
    uint64_t mask = mask_from_sf(sf_dpimm);
    uint64_t entry = 0;

    if (opi_dpimm == OPI_DPIMM_ARITH) {
        uint32_t rn_arith    = extract_bits(RN_DPIMM_ARITH_START,
                                            RN_DPIMM_ARITH_END, instr);
        uint32_t shift_arith = extract_bits(SH_DPIMM_ARITH_START,
                                            SH_DPIMM_ARITH_END, instr);
        uint64_t operand2    = extract_bits(IMM12_DPIMM_START,
                                            IMM12_DPIMM_END, instr);
        uint64_t rn_value    = read_dp_register(rn_arith) & mask;
        
        if (shift_arith) {
            operand2 <<= DPIMM_ARITH_SHIFT_AMOUNT;
        }
        operand2 &= mask;
        entry = compute_add_sub_result(opc_dpimm, rn_value, operand2, mask, 
                                       sf_dpimm);
        write_dp_result(rd_dpimm, sf_dpimm, entry);
    } else if (opi_dpimm == OPI_DPIMM_WIDE_MOVE) {
        uint32_t hw_wm = extract_bits(SH_DPIMM_WM_START, SH_DPIMM_WM_END,
                                      instr);
        uint64_t imm16_wm = extract_bits(IMM16_DPIMM_WM_START,
                                         IMM16_DPIMM_WM_END, instr);
        uint32_t shift = DPIMM_WIDE_MOVE_SHIFT_FACTOR * hw_wm;
        uint64_t shifted = (imm16_wm << shift) & mask;
        switch (opc_dpimm) {
            case OPC_DPIMM_MOVN:
                entry = (~shifted) & mask;
                break;
            case OPC_DPIMM_MOVZ: 
                entry = shifted;
                break;
            case OPC_DPIMM_MOVK:
                uint64_t old = read_dp_register(rd_dpimm) & mask;
                uint64_t clear_mask = ~((uint64_t)DP_16BIT_MASK << shift) & mask;
                entry = (old & clear_mask) | shifted;
                break;
            default:
                fprintf(stderr, "invalid opcode for dp imm widemove\n");
                exit(EXIT_FAILURE);
                return;
        } 
        write_dp_result(rd_dpimm, sf_dpimm, entry);
    }
}
