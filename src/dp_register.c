#include "dp_shared.h"
#include "dp_register.h"

static uint64_t shift_operand(uint64_t operand, uint32_t shift_type,
                              uint32_t shift_dist, uint32_t sf) {
    uint64_t mask = mask_from_sf(sf);
    uint32_t width = sf == 0 ? 32 : 64;
    operand &= mask;
    if (shift_dist == 0) {
        return operand;
    }

    if (shift_type == SHIFT_TYPE_LSL) {
        return (operand << shift_dist) & mask;
    }
    if (shift_type == SHIFT_TYPE_LSR) {
        return operand >> shift_dist;
    }
    if (shift_type == SHIFT_TYPE_ASR) {
        if (sf == 0) {
            return (uint32_t)((int32_t)operand >> shift_dist);
        }
        return (uint64_t)((int64_t)operand >> shift_dist);
    }

    shift_dist %= width;
    if (shift_dist == 0) {
        return operand;
    }
    return ((operand >> shift_dist) | (operand << (width - shift_dist))) & mask;
}

static void set_logic_flags(uint64_t entry, uint32_t sf) {
    uint64_t masked = entry & mask_from_sf(sf);

    if (sf == 0) {
        update_negative_flag32(&pState, masked);
    } else {
        update_negative_flag64(&pState, masked);
    }
    update_zero_flag(&pState, masked);
    pState.c = false;
    pState.v = false;
}

void dpreg(uint32_t instr) {
    pState = (state){false, false, false, false};

    uint32_t sf_dpreg = bitmask_check(SF_DPREG_START, SF_DPREG_END, instr);
    uint32_t opc_dpreg = bitmask_check(OPC_DPREG_START, OPC_DPREG_END, instr);
    uint32_t m_dpreg = bitmask_check(M_DPREG_START, M_DPREG_END, instr);
    uint32_t rd_index = bitmask_check(RD_DPREG_START, RD_DPREG_END, instr);
    uint32_t rn_index = bitmask_check(RN_DPREG_START, RN_DPREG_END, instr);
    uint32_t rm_index = bitmask_check(RM_DPREG_START, RM_DPREG_END, instr);
    uint64_t mask = mask_from_sf(sf_dpreg);
    uint64_t rn = read_dp_register(rn_index, *registers) & mask;
    uint64_t rm = read_dp_register(rm_index, *registers) & mask;
    uint64_t entry = 0;

    if (m_dpreg == M_DPREG_MULTIPLY) {
        uint32_t ra_index = bitmask_check(RA_DPREG_MULT_START,
                                          RA_DPREG_MULT_END, instr);
        uint32_t x_mult = bitmask_check(X_DPREG_MULT_START,
                                        X_DPREG_MULT_END, instr);
        uint64_t ra_mult = read_dp_register(ra_index, *registers) & mask;
        uint64_t product = (rn * rm) & mask;

        if (x_mult == X_DPREG_MADD) {
            entry = (ra_mult + product) & mask;
        } else {
            entry = (ra_mult - product) & mask;
        }

        write_dp_result(rd_index, sf_dpreg, entry, *registers);
    } else {
        uint32_t opr0 = bitmask_check(OPR0_DPREG_START, OPR0_DPREG_END,
                                      instr);
        uint32_t shift_type = bitmask_check(SHIFT_DPREG_START,
                                            SHIFT_DPREG_END, instr);
        uint32_t shift_dist = bitmask_check(SHIFT_DIST_DPREG_START,
                                            SHIFT_DIST_DPREG_END, instr);
        uint64_t operand2 = shift_operand(rm, shift_type, shift_dist, sf_dpreg);

        if (opr0 == OPR0_DPREG_ARITH) {
            if (opc_dpreg < DP_OPC_SUBTRACT_START) {
                entry = (rn + operand2) & mask;
                if (opc_dpreg == DP_OPC_ADD_SETFLAG) {
                    if (sf_dpreg == 0) {
                        add32flags(rn_index, entry, &pState, operand2);
                    } else {
                        add64flags(rn_index, entry, &pState, operand2);
                    }
                }
            } else {
                entry = (rn - operand2) & mask;
                if (opc_dpreg == DP_OPC_SUB_SETFLAG) {
                    if (sf_dpreg == 0) {
                        sub32flags(rn_index, entry, &pState, operand2);
                    } else {
                        sub64flags(rn_index, entry, &pState, operand2);
                    }
                }
            }
        } else {
            if (bitmask_check(N_DPREG_START, N_DPREG_END, instr) == 1) {
                operand2 = ~operand2 & mask;
            }

            switch (opc_dpreg) {
                case OPC_DPREG_AND:
                    entry = rn & operand2;
                    break;
                case OPC_DPREG_ORR:
                    entry = rn | operand2;
                    break;
                case OPC_DPREG_EOR:
                    entry = rn ^ operand2;
                    break;
                default:
                    entry = rn & operand2;
                    set_logic_flags(entry, sf_dpreg);
                    break;
            }
        }
        write_dp_result(rd_index, sf_dpreg, entry, *registers);
    }
}