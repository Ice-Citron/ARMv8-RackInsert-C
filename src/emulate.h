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

extern uint8_t memory[MEM_SIZE];
extern uint64_t registers[REGS];
extern uint64_t pc;
extern state pState; // = {false, false, false, false};

#endif