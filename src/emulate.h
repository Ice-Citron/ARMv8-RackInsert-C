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

typedef struct {
    bool n;
    bool z;
    bool c;
    bool v;
} state;

//initial flags
#define INITIAL_PSTATE ((state){ .n = false, .z = true, .c = false, .v = false})

extern uint8_t memory[MEM_SIZE];
extern uint64_t registers[REGS];
extern uint64_t pc;
extern state pState;

//obtains sign for 32 bit mode
static inline uint8_t sign32 (uint64_t target) { return (target >> 31) & 1; }
//obtains sign for 64 bit mode
static inline uint8_t sign64 (uint64_t target) { return (target >> 63) & 1; }

#endif
