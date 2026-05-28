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

#define UNSIGNED_IMM_OFFSET 1
#define IS_32BIT_RES 1
#define IS_LOAD_OP 1

void single_data_transfer(uint32_t instr)
{
    clearstate(&pState);
    uint32_t RtAddr = bitmask_check(0, 4, instr);
    uint32_t baseRegAddr = bitmask_check(5, 9, instr);
    uint32_t sizeToggle = bitmask_check(30, 30, instr);
    if (bitmask_check(24, 24, instr) == UNSIGNED_IMM_OFFSET) // unsigned immediate offset
    {
        uint32_t imm12 = bitmask_check(10, 21, instr);
        uint64_t target = registers[baseRegAddr];
        if (sizeToggle == IS_32BIT_RES) // sign flag is 1 so we're changing in 32 bit mode
        {
            target += (imm12 << 2); // imm12 * 4
        }
        else
        {
            target += (imm12 << 3); // imm12 * 8
        }
        if (bitmask_check(22, 22, instr) == IS_LOAD_OP) // load operation
        {
            registers[RtAddr] = 0;
            int n = 8;
            if (sizeToggle == IS_32BIT_RES)
            {
                n = 4;
            }
            for (int i = 0; i < n; i++) // n = 8 bytes for 64 bits and n = 4 for 32 bits
            {
                registers[RtAddr] |= ( memory[target+i] << (i * 8)); // plug in 8 bits at a time
            }
        }
        else // store operation
        {
            if (sizeToggle == IS_32BIT_RES)
            {   // preserve last 32 bits
                memory[target] = bitmask_check(63, 32, memory[target]) << 32;
                memory[target] |= registers[RtAddr];
            }
            else
            {
                memory[target] = registers[RtAddr];
            }
        }
    }
}