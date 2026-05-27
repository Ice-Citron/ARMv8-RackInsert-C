#ifndef EMULATOR_EMULATE_H
#define EMULATOR_EMULATE_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define MEM_SIZE (1 << 21)
#define REGS 32
#define ZERO_REGISTER_INDEX 31
#define HALT_INSTRUCTION 0x8a000000

uint8_t memory[MEM_SIZE];
uint64_t registers[REGS];
uint64_t pc;

typedef struct {
    bool n;
    bool z;
    bool c;
    bool v;
} state;

state pState = {false, false, false, false};

static inline uint32_t bitmask (uint8_t start, uint8_t end) {
    //generates the bitmask which starts at start and ends at end
    uint8_t width = start - end + 1;

    if (width == 32) {
        return UINT32_MAX;
    }
    return ((1u << width) - 1u) << end;
}

static inline uint32_t bitmask_check (uint8_t start, uint8_t end, uint32_t target) {
    return (bitmask(start, end) & target) >> end;
}

static inline uint8_t sign32 (uint64_t target) {
    return bitmask_check(31, 31, target);
}

static inline uint8_t sign64 (uint64_t target) {
    return (target >> 63) & 1;
}

static inline void update_zero_flag(state* state, uint64_t target) {
    state->z = target == 0;
}

static inline void update_negative_flag32(state* state, uint64_t target) {
    state->n = sign32(target) == 1;
}

static inline void update_negative_flag64(state* state, uint64_t target) {
    state->n = sign64(target) == 1;
}

static uint64_t read_dp_register(uint32_t regindex) {
    return regindex == ZERO_REGISTER_INDEX ? 0 : registers[regindex];
}

static uint64_t flag_register_value(uint32_t regindex) {
    return read_dp_register(regindex);
}

#endif
