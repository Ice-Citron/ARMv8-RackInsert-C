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
    return ((1 << (start - end + 1)) - 1) << end;
}

uint32_t bitmask_check (uint8_t start, uint8_t end, uint32_t target) {
    return (bitmask(start, end) & target) >> end;
}

uint8_t sign32 (uint64_t target) {
    return bitmask_check(31, 31, target);
}

uint8_t sign64 (uint64_t target) {
    return bitmask_check(63, 63, target);
}

#define DATA_TRANSFER_LOAD_OP 1
#define IS_32BIT_RES 1

void single_data_transfer(uint32_t instr)
{
    clearstate(&pState);
    uint32_t targetRegAddr = bitmask_check(0, 4, instr);
    uint32_t sizeToggle = bitmask_check(30, 30, instr);
    if (bitmask_check(22, 22, instr) == DATA_TRANSFER_LOAD_OP) // load operation
    {

    }
}