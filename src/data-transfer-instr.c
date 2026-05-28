#include "dp.h"
#define UNSIGNED_IMM_OFFSET 1
#define PRE_POST_INDEXED 1
#define PRE_INDEXED 1
#define IS_32BIT_RES 1
#define IS_LOAD_OP 1

void load_operation(uint32_t rtAddr, uint64_t target, int n)
{
    registers[rtAddr] = 0;
    for (int i = 0; i < n; i++) // n = 8 bytes for 64 bits and n = 4 for 32 bits
    {
        registers[rtAddr] |= ((uint64_t) memory[target+i] << (i * 8)); // plug in 8 bits at a time
    }
}

void store_operation(uint32_t rtAddr, uint64_t target, int n)
{
    for (int i = 0; i < n; i++) // n = 8 bytes for 64 bits and n = 4 for 32 bits
    {
        // code to copy value of bits in registers into the memory locations
        memory[target+i] = (uint8_t)(registers[rtAddr] >> (i * 8)) & 0xff; // will implement later in a helper
    }
}
void handle_operation(uint32_t rtAddr, uint64_t target, int n, uint32_t operation)
{
    if (operation == IS_LOAD_OP)
    {
        load_operation(rtAddr, target, n);
    }
    else
    {
        store_operation(rtAddr, target, n);
    }
}

void single_data_transfer(uint32_t instr)
{
    clearstate(&pState);
    uint32_t rtAddr = bitmask_check(0, 4, instr);
    uint32_t xnAddr = bitmask_check(5, 9, instr);
    uint32_t sizeToggle = bitmask_check(30, 30, instr);
    uint32_t operation = bitmask_check(22, 22, instr) ;
    uint64_t target;
    int n = 8;
    if (sizeToggle == IS_32BIT_RES) // sf is 1 so we're changing in 32 bit mode
    {
        n = 4;
    }
    if (bitmask_check(24, 24, instr) == UNSIGNED_IMM_OFFSET) // unsigned immediate offset
    {
        uint32_t imm12 = bitmask_check(10, 21, instr);
        target = registers[xnAddr];
        if (sizeToggle == IS_32BIT_RES) // sf is 1 so we're changing in 32 bit mode
        {
            target += (imm12 << 2); // imm12 * 4
            n = 4;
        }
        else
        {
            target += (imm12 << 3); // imm12 * 8
        }
        handle_operation(rtAddr, target, n, operation);
    }
    else if (bitmask_check(10, 10, instr) == PRE_POST_INDEXED)
    {
        int32_t simm9 = bitmask_check(12, 20, instr);
        target = xnAddr + simm9;
        // we need proper methods of extracting bits, I don't think returning uint32 is sufficient every time
        if (bitmask_check(11, 11, instr) == PRE_INDEXED)
        {
            registers[xnAddr] = target;
            handle_operation(rtAddr, target, n, operation);
        }
        else // POST_INDEXED
        {
            handle_operation(rtAddr, target, n, operation);
            registers[xnAddr] = target;
        }
    }
}