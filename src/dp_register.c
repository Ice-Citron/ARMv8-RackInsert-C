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
            uint32_t answer = ((int32_t)operand >> shift_dist);
            if (extract_bits(31, 31, operand) == 1) {
                answer = answer | (bitmask(shift_dist, 1) << (31 - shift_dist));
            }
            return answer;
        } else {
            uint64_t answer = ((int64_t)operand >> shift_dist);
            if (extract_bits(63, 63, operand) == 1) {
                answer = answer | (bitmask(shift_dist, 1) << (63 - shift_dist));
            }
            return answer;
        }
    }
    if (shift_type == SHIFT_TYPE_ROR) {
        if (sf == 0) {
            return (extract_bits(shift_dist - 1, 0, operand) <<
            (32 - shift_dist)) | extract_bits(31, shift_dist, operand);
        } else {
            return (extract_bits(shift_dist - 1, 0, operand) <<
            (64 - shift_dist)) | extract_bits(63, shift_dist, operand);
        }
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

    uint32_t sf_dpreg = extract_bits(SF_DPREG_START, SF_DPREG_END, instr);
    uint32_t opc_dpreg = extract_bits(OPC_DPREG_START, OPC_DPREG_END, instr);
    uint32_t m_dpreg = extract_bits(M_DPREG_START, M_DPREG_END, instr);
    uint32_t rd_index = extract_bits(RD_DPREG_START, RD_DPREG_END, instr);
    uint32_t rn_index = extract_bits(RN_DPREG_START, RN_DPREG_END, instr);
    uint32_t rm_index = extract_bits(RM_DPREG_START, RM_DPREG_END, instr);
    uint64_t mask = mask_from_sf(sf_dpreg);
    uint64_t rn = read_dp_register(rn_index) & mask;
    uint64_t rm = read_dp_register(rm_index) & mask;
    uint64_t entry = 0;

    if (m_dpreg == M_DPREG_MULTIPLY) {
        uint32_t ra_index = extract_bits(RA_DPREG_MULT_START,
                                          RA_DPREG_MULT_END, instr);
        uint32_t x_mult = extract_bits(X_DPREG_MULT_START,
                                        X_DPREG_MULT_END, instr);
        uint64_t ra_mult = read_dp_register(ra_index) & mask;
        uint64_t product = (rn * rm) & mask;

        if (x_mult == X_DPREG_MADD) {
            entry = (ra_mult + product) & mask;
        } else {
            entry = (ra_mult - product) & mask;
        }

        write_dp_result(rd_index, sf_dpreg, entry);
    } else {
        uint32_t opr0 = extract_bits(OPR0_DPREG_START, OPR0_DPREG_END,
                                      instr);
        uint32_t shift_type = extract_bits(SHIFT_DPREG_START,
                                            SHIFT_DPREG_END, instr);
        uint32_t shift_dist = extract_bits(SHIFT_DIST_DPREG_START,
                                            SHIFT_DIST_DPREG_END, instr);
        uint64_t operand2 = shift_operand(rm, shift_type, shift_dist, sf_dpreg);
        if (shift_type == SHIFT_TYPE_ROR && opr0 == OPR0_DPREG_ARITH) {
            fprintf(stderr, "invalid: rotate right with arithmetic op");
            exit(1);
        }
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
            if (extract_bits(N_DPREG_START, N_DPREG_END, instr) == 1) {
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
        write_dp_result(rd_index, sf_dpreg, entry);
    }
}