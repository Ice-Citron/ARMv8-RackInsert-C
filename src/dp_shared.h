#ifndef DP_SHARED_H
#define DP_SHARED_H

#include "bit_manipulation.h"
#include "emulate.h"

#define DP_16BIT_MASK         UINT16_MAX

void dpimm(uint32_t instr);
void dpreg(uint32_t instr);
void write_dp_result(uint32_t rd, uint32_t sf, uint64_t entry);

static inline void clear_pstate_flags(void) {
    pState.n = false;
    pState.z = false;
    pState.c = false;
    pState.v = false;
}

static inline uint64_t mask_from_sf(uint32_t sf) { 
    return sf == 0 ? UINT32_MAX : UINT64_MAX; 
}

static inline void update_zero_flag(uint64_t target) {
    pState.z = target == 0;
}

static inline void update_negative_flag(uint64_t target, uint32_t sf) {
    if (sf == 0) { 
        pState.n = sign32(target); 
    } else {
        pState.n = sign64(target);
    }
}

static inline uint64_t read_dp_register(uint32_t regindex) {
    return regindex == ZERO_REGISTER_INDEX ? 0 : registers[regindex];
}

static inline uint64_t flag_register_value(uint32_t regindex) {
    return read_dp_register(regindex);
}

uint64_t compute_add_sub_result(uint32_t opc, uint64_t rn, uint64_t operand2, 
                                uint64_t mask, uint32_t sf);

#endif
