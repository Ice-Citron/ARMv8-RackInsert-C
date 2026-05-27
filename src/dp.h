#ifndef DP_H
#define DP_H

#include "emulate.h"

#define DP_CLEAR_LOWER_32_MASK (~(uint64_t) UINT32_MAX)
#define DP_WIDE_MOVE_IMM_MASK UINT16_MAX

#define DP_OPC_ADD 0
#define DP_OPC_ADDS 1
#define DP_OPC_SUBTRACT_START 2
#define DP_OPC_SUBS 3

#define SF_DPIMM_START 31
#define SF_DPIMM_END 31
#define OPC_DPIMM_START 30
#define OPC_DPIMM_END 29
#define OPI_DPIMM_START 25
#define OPI_DPIMM_END 23
#define RD_DPIMM_START 4
#define RD_DPIMM_END 0

#define OPI_DPIMM_ARITH 2
#define RN_DPIMM_ARITH_START 9
#define RN_DPIMM_ARITH_END 5
#define SH_DPIMM_ARITH_START 22
#define SH_DPIMM_ARITH_END 22
#define IMM12_DPIMM_START 21
#define IMM12_DPIMM_END 10
#define DPIMM_ARITH_SHIFT_AMOUNT 12

#define OPI_DPIMM_WIDE_MOVE 5
#define SH_DPIMM_WM_START 22
#define SH_DPIMM_WM_END 21
#define IMM16_DPIMM_WM_START 20
#define IMM16_DPIMM_WM_END 5
#define DPIMM_WIDE_MOVE_SHIFT_FACTOR 16
#define OPC_DPIMM_MOVN 0
#define OPC_DPIMM_MOVZ 2
#define OPC_DPIMM_MOVK 3

#define SF_DPREG_START 31
#define SF_DPREG_END 31
#define OPC_DPREG_START 30
#define OPC_DPREG_END 29
#define M_DPREG_START 28
#define M_DPREG_END 28
#define RD_DPREG_START 4
#define RD_DPREG_END 0
#define RN_DPREG_START 9
#define RN_DPREG_END 5
#define RM_DPREG_START 20
#define RM_DPREG_END 16

#define M_DPREG_MULTIPLY 1
#define RA_DPREG_MULT_START 14
#define RA_DPREG_MULT_END 10
#define X_DPREG_MULT_START 15
#define X_DPREG_MULT_END 15
#define X_DPREG_MADD 0

#define OPR0_DPREG_START 24
#define OPR0_DPREG_END 24
#define OPR0_DPREG_ARITH 1
#define SHIFT_DPREG_START 23
#define SHIFT_DPREG_END 22
#define SHIFT_DIST_DPREG_START 15
#define SHIFT_DIST_DPREG_END 10
#define N_DPREG_START 21
#define N_DPREG_END 21

#define SHIFT_TYPE_LSL 0
#define SHIFT_TYPE_LSR 1
#define SHIFT_TYPE_ASR 2

#define OPC_DPREG_AND 0
#define OPC_DPREG_ORR 1
#define OPC_DPREG_EOR 2

void dpimm(uint32_t instr);
void dpreg(uint32_t instr);

static uint32_t get32from64(uint64_t target) {
    return (uint32_t) target;
}

static inline void add32flags(uint32_t regindex, uint64_t target, state* state, uint64_t operand2) {
    update_zero_flag(state, target);
    update_negative_flag32(state, target);
    state->c = (uint32_t) operand2 > ~get32from64(flag_register_value(regindex));
    state->v = sign32(flag_register_value(regindex)) == 0 && sign32(target) == 1;
}

static inline void add64flags(uint32_t regindex, uint64_t target, state* state, uint64_t operand2) {
    update_zero_flag(state, target);
    update_negative_flag64(state, target);
    state->c = (uint64_t) operand2 > ~flag_register_value(regindex);
    state->v = sign64(flag_register_value(regindex)) == 0 && sign64(target) == 1;
}

static inline void sub32flags(uint32_t regindex, uint64_t target, state* state, uint64_t operand2) {
    update_zero_flag(state, target);
    update_negative_flag32(state, target);
    state->c = (uint32_t) operand2 > get32from64(flag_register_value(regindex));
    state->v = sign32(flag_register_value(regindex)) == 1 && sign32(target) == 0;
}

static inline void sub64flags(uint32_t regindex, uint64_t target, state* state, uint64_t operand2) {
    update_zero_flag(state, target);
    update_negative_flag64(state, target);
    state->c = (uint64_t) operand2 > flag_register_value(regindex);
    state->v = sign64(flag_register_value(regindex)) == 1 && sign64(target) == 0;
}

static uint64_t mask_from_sf(uint32_t sf) {
    return sf == 0 ? UINT32_MAX : UINT64_MAX;
}

static void write_dp_result(uint32_t rd, uint32_t sf, uint64_t entry) {
    if (rd == ZERO_REGISTER_INDEX) {
        return;
    }

    if (sf == 0) {
        registers[rd] = (registers[rd] & DP_CLEAR_LOWER_32_MASK)
                      | (entry & UINT32_MAX);
    } else {
        registers[rd] = entry;
    }
}

#endif