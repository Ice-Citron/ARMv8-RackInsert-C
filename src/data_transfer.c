#include "data_transfer.h"

void load_operation(const uint32_t rtAddr, const uint64_t target, const int n) {
    registers[rtAddr] = 0;
    for (int i = 0; i < n; i++) // n = 8 bytes for 64 bits and n = 4 for 32 bits
    {
        registers[rtAddr] |= ((uint64_t) memory[target+i] << (i * 8)); // plug in 8 bits at a time
    }
}

void store_operation(const uint32_t rtAddr, const uint64_t target, const int n) {
    for (int i = 0; i < n; i++) // n = 8 bytes for 64 bits and n = 4 for 32 bits
    {
        // code to copy value of bits in registers into the memory locations
        memory[target+i] = (uint8_t)(registers[rtAddr] >> (i * 8)) & 0xff; // will implement later in a helper
    }
}
void perform_load_or_store(const uint32_t rtAddr, const uint64_t target, 
                           const int n, const uint32_t operation) {
    if (operation == IS_LOAD_OP) {
        load_operation(rtAddr, target, n);
    }
    else {
        store_operation(rtAddr, target, n);
    }
}

void single_data_transfer(const uint32_t instr) {
    const uint32_t rtAddr = extract_bits(4, 0, instr);
    const uint32_t sizeToggle = extract_bits(30, 30, instr);
    int numOfBytes = 8;
    if (sizeToggle == IS_32BIT_RES) // sf is 1 so we're changing in 32 bit mode
    {
        numOfBytes = 4;
    }
    if (extract_bits(31, 31, instr) == IS_SINGLE_DATA_TRANSFER)
    {
        const uint32_t xnAddr = extract_bits(9, 5, instr);
        const uint32_t operation = extract_bits(22, 22, instr) ;
        uint64_t target = registers[xnAddr];
        if (extract_bits(24, 24, instr) == UNSIGNED_IMM_OFFSET) // unsigned immediate offset
        {
            const uint32_t imm12 = extract_bits(21, 10, instr);
            if (sizeToggle == IS_32BIT_RES) // sf is 1 so we're changing in 32 bit mode
            {
                target += (imm12 << 2); // imm12 * 4
            }
            else
            {
                target += (imm12 << 3); // imm12 * 8
            }
            perform_load_or_store(rtAddr, target, numOfBytes, operation);
        }
        else if (extract_bits(10, 10, instr) == PRE_POST_INDEXED)
        {
            const int simm9 = get_signed_value_from_bits(20, 12, instr);
            if (extract_bits(11, 11, instr) == PRE_INDEXED)
            {
                target += simm9;
                registers[xnAddr] = target;
                perform_load_or_store(rtAddr, target, numOfBytes, operation);
            }
            else // POST_INDEXED
            {
                perform_load_or_store(rtAddr, target, numOfBytes, operation);
                target += simm9;
                registers[xnAddr] = target;
            }
        }
        else if (extract_bits(15, 10, instr) == IS_REG_OFFSET_MODE) // register offset
        {
            const uint32_t xmAddr = extract_bits(20, 16, instr);
            target += registers[xmAddr]; // might need to have a check that xm is <=30
            perform_load_or_store(rtAddr, target, numOfBytes, operation);
        }
    }
    else // IS_LOAD_LITERAL
    {
        const uint32_t simm19 = extract_bits(23, 5, instr);
        uint64_t offset = simm19 << 2; // simm19 * 4
        if (extract_bits(20, 20, offset) & 1)
        {
            offset |= 0xfffffffffff00000; // sign extend to 64
        }
        const uint64_t target = pc + offset;
        load_operation(rtAddr, target, numOfBytes);
    }
}