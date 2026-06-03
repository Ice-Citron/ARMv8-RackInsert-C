#include "dp_register.h"

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
        return ((operand >> shift_dist) | (operand << (width - shift_dist))) & mask;
    } else {
        fprintf(stderr, "ERROR: Invalid shift type/dist detected.");
        exit(EXIT_FAILURE);
    }
}

static void set_logical_flags(uint64_t entry, uint32_t sf) {
    uint64_t masked = entry & mask_from_sf(sf);
    update_negative_flag(masked, sf);
    update_zero_flag(masked);
    pState.c = false;
    pState.v = false;
}

static uint64_t compute_multiply_result(uint32_t instr, uint64_t rn, 
                                        uint64_t rm, uint64_t mask) {
    uint64_t entry = 0;
    uint32_t ra_index = extract_bits(RA_DPREG_MULT_START,
                                     RA_DPREG_MULT_END, instr);
    uint32_t x_mult   = extract_bits(X_DPREG_MULT_START,
                                     X_DPREG_MULT_END, instr);
    uint64_t ra_mult = read_dp_register(ra_index) & mask;
    uint64_t product = (rn * rm) & mask;

    if (x_mult == X_DPREG_MADD) {
        entry = (ra_mult + product) & mask;
    } else {
        entry = (ra_mult - product) & mask;
    }
    return entry;
}

static uint64_t compute_logical_result(uint32_t instr, uint32_t opcode, 
                                       uint64_t rn, uint64_t operand2, 
                                       uint32_t sf) {
    uint64_t entry = 0;
    uint64_t mask = mask_from_sf(sf);
    if (extract_bits(N_DPREG_START, N_DPREG_END, instr) == 1) {
        operand2 = ~operand2 & mask;
    }
    switch (opcode) {
        case OPC_DPREG_AND:
            entry = rn & operand2;
            break;
        case OPC_DPREG_ORR:
            entry = rn | operand2;
            break;
        case OPC_DPREG_EOR:
            entry = rn ^ operand2;
            break;
        case OPC_DPREG_AND_BRICS:
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

void dpreg(uint32_t instr) {
    uint32_t sf_dpreg  = extract_bits(SF_DPREG_START , SF_DPREG_END , instr);
    uint32_t opc_dpreg = extract_bits(OPC_DPREG_START, OPC_DPREG_END, instr);
    uint32_t m_dpreg   = extract_bits(M_DPREG_START  , M_DPREG_END  , instr);
    uint32_t rd_index  = extract_bits(RD_DPREG_START , RD_DPREG_END , instr);
    uint32_t rn_index  = extract_bits(RN_DPREG_START , RN_DPREG_END , instr);
    uint32_t rm_index  = extract_bits(RM_DPREG_START , RM_DPREG_END , instr);
    uint64_t mask = mask_from_sf(sf_dpreg);
    uint64_t rn = read_dp_register(rn_index) & mask;
    uint64_t rm = read_dp_register(rm_index) & mask;
    uint64_t entry = 0;

    if (m_dpreg == M_DPREG_MULTIPLY) {
        entry = compute_multiply_result(instr, rn, rm, mask);
        write_dp_result(rd_index, sf_dpreg, entry);
    } else {
        uint32_t opr0 = extract_bits(OPR0_DPREG_START, OPR0_DPREG_END, instr);
        uint32_t shift_type = extract_bits(SHIFT_DPREG_START,
                                           SHIFT_DPREG_END, instr);
        uint32_t shift_dist = extract_bits(SHIFT_DIST_DPREG_START,
                                           SHIFT_DIST_DPREG_END, instr);
        uint64_t operand2 = shift_operand(rm, shift_type, shift_dist, sf_dpreg);
        if (shift_type == SHIFT_TYPE_ROR && opr0 == OPR0_DPREG_ARITH) {
            fprintf(stderr, "ERROR: Invalid rotate right with arithmetic op\n");
            exit(EXIT_FAILURE);
        }
        if (opr0 == OPR0_DPREG_ARITH) {
            entry = compute_add_sub_result(opc_dpreg, rn, operand2, mask, 
                                           sf_dpreg);
        } else {
            entry = compute_logical_result(instr, opc_dpreg, rn, operand2, sf_dpreg);
        }
        write_dp_result(rd_index, sf_dpreg, entry);
    }
}
