#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define MEM_SIZE (1 << 21)
#define REGS 31

uint8_t memory[MEM_SIZE];
uint64_t registers[REGS];
uint64_t pc;

const uint64_t zeroRegister = 0;

typedef struct {
    bool n;
    bool z;
    bool c;
    bool v;
} state;

state pState = {false, false, false, false};

uint32_t bitmask (uint8_t start, uint8_t end) {
    //generates the bitmask which starts at start and ends at end
    uint8_t width = start - end + 1;

    if (width == 32) {
        return UINT32_MAX;
    }
    return ((1u << width) - 1u) << end;
}

uint32_t bitmask_check (uint8_t start, uint8_t end, uint32_t target) {
    return (bitmask(start, end) & target) >> end;
}

uint8_t sign32 (uint64_t target) {
    return bitmask_check(31, 31, target);
}

uint8_t sign64 (uint64_t target) {
    return (target >> 63) & 1;
}
