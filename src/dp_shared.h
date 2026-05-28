#include "bit_manipulation.h"
#include "emulate.h"

#define DP_CLEAR_LOWER_32_MASK (~(uint64_t) UINT32_MAX)

#define DP_OPC_ADD 0
#define DP_OPC_ADD_SETFLAG 1
#define DP_OPC_SUBTRACT_START 2
#define DP_OPC_SUB_SETFLAG 3

#define DP_16BIT_MASK UINT16_MAX

static uint32_t get32from64(uint64_t target) {return (uint32_t) target;}
static uint64_t mask_from_sf(uint32_t sf) {return sf == 0 ? UINT32_MAX : UINT64_MAX;}
void write_dp_result(uint32_t rd, uint32_t sf, uint64_t entry, uint64_t* regs);

static inline void update_zero_flag(state* state, uint64_t target) {
    state->z = target == 0;
}

static inline void update_negative_flag32(state* state, uint64_t target) {
    state->n = sign32(target) == 1;
}

static inline void update_negative_flag64(state* state, uint64_t target) {
    state->n = sign64(target) == 1;
}

static uint64_t read_dp_register(uint32_t regindex, uint64_t* regs) {
    return regindex == ZERO_REGISTER_INDEX ? 0 : regs[regindex];
}