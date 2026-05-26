#include "dp.h"

void dpimm(uint32_t instr) {
    pState = (state){false, false, false, false};

    uint32_t sf_dpimm = bitmask_check(SF_DPIMM_START, SF_DPIMM_END, instr);
    uint32_t opc_dpimm = bitmask_check(OPC_DPIMM_START, OPC_DPIMM_END, instr);
    uint32_t opi_dpimm = bitmask_check(OPI_DPIMM_START, OPI_DPIMM_END, instr);
    uint32_t rd_dpimm = bitmask_check(RD_DPIMM_START, RD_DPIMM_END, instr);
    uint64_t mask = mask_from_sf(sf_dpimm);
    uint64_t entry = 0;

    if (opi_dpimm == OPI_DPIMM_ARITH) {
        uint32_t rn_arith = bitmask_check(RN_DPIMM_ARITH_START,
                                          RN_DPIMM_ARITH_END, instr);
        uint32_t shift_arith = bitmask_check(SH_DPIMM_ARITH_START,
                                             SH_DPIMM_ARITH_END, instr);
        uint64_t operand2 = bitmask_check(IMM12_DPIMM_START,
                                          IMM12_DPIMM_END, instr);
        uint64_t rn_value = read_dp_register(rn_arith) & mask;

        if (shift_arith) {
            operand2 <<= DPIMM_ARITH_SHIFT_AMOUNT;
        }
        operand2 &= mask;

        if (opc_dpimm < DP_OPC_SUBTRACT_START) {
            entry = (rn_value + operand2) & mask;
            if (opc_dpimm == DP_OPC_ADDS) {
                if (sf_dpimm == 0) {
                    add32flags(rn_arith, entry, &pState, operand2);
                } else {
                    add64flags(rn_arith, entry, &pState, operand2);
                }
            }
        } else {
            entry = (rn_value - operand2) & mask;
            if (opc_dpimm == DP_OPC_SUBS) {
                if (sf_dpimm == 0) {
                    sub32flags(rn_arith, entry, &pState, operand2);
                } else {
                    sub64flags(rn_arith, entry, &pState, operand2);
                }
            }
        }

        write_dp_result(rd_dpimm, sf_dpimm, entry);
    } else if (opi_dpimm == OPI_DPIMM_WIDE_MOVE) {
        uint32_t hw_wm = bitmask_check(SH_DPIMM_WM_START, SH_DPIMM_WM_END,
                                       instr);
        uint64_t imm16_wm = bitmask_check(IMM16_DPIMM_WM_START,
                                          IMM16_DPIMM_WM_END, instr);
        uint32_t shift = DPIMM_WIDE_MOVE_SHIFT_FACTOR * hw_wm;
        uint64_t shifted = (imm16_wm << shift) & mask;
        
        switch (opc_dpimm) {
        case OPC_DPIMM_MOVN:
            entry = shifted | mask;
            break;
        case OPC_DPIMM_MOVZ: 
            entry = shifted;
            break;
        case OPC_DPIMM_MOVK:
            uint64_t mask_16_bits = ~((uint64_t) DP_WIDE_MOVE_IMM_MASK << shift);
            entry = (read_dp_register(rd_dpimm) | mask_16_bits) & shifted;
            break;
        default:
            return;
        } 
        rd_dpimm = entry;
    }
}