#include "data_transfer.h"

static void load_operation(const uint32_t rt_index, const uint64_t target, 
                                  const int n) {
    registers[rt_index] = 0;
    for (int i = 0; i < n; i++) {
        uint64_t memAddr = target + (uint64_t)i;    
        registers[rt_index] |= ((uint64_t)memory[memAddr] << (i * 8)); // plug in 8 bits at a time
    }
}

static void store_operation(const uint32_t rt_index, const uint64_t target, 
                                   const int n) {
    for (int i = 0; i < n; i++) { // n = 8 bytes for 64 bits; n = 4 for 32 bits
        uint64_t memAddr = target + (uint64_t)i;
        memory[memAddr] = (uint8_t)(registers[rt_index] >> (i * 8)) & 0xff; 
    }
}

static void perform_load_or_store(const uint32_t rt_index, 
                                         const uint64_t target, const int n, 
                                         const uint32_t operation) {
    if (operation == DT_LOAD_OP) {
        load_operation(rt_index, target, n);
    }
    else {
        store_operation(rt_index, target, n);
    }
}

void single_data_transfer(const uint32_t instr) {
    const uint32_t rt_index = extract_bits(DT_RT_HI, DT_RT_LO, instr);
    const uint32_t size     = extract_bits(DT_SIZE_HI, DT_SIZE_LO, instr);
    int num_bytes = 8;
    if (size == DT_32BIT_MODE) {
        num_bytes = 4;
    }
    if (extract_bits(DT_SINGLE_HI, DT_SINGLE_LO, instr) == DT_SINGLE_MODE) {
        const uint32_t xn_index = extract_bits(DT_XN_HI, DT_XN_LO, instr);
        const uint32_t operation = extract_bits(DT_OP_HI, DT_OP_HI, instr) ;
        uint64_t target = registers[xn_index];
        if (extract_bits(DT_UINT_HI, DT_UINT_LO, instr) == DT_UINT_IMM_OFFSET) {
            const uint32_t imm12 = extract_bits(DT_IMM12_HI, DT_IMM12_LO, 
                                                instr);
            if (size == DT_32BIT_MODE) {
                target += (imm12 << 2);
            } else {
                target += (imm12 << 3);
            }
            perform_load_or_store(rt_index, target, num_bytes, operation);
        }
        else if (extract_bits(DT_PP_INDEX_HI, DT_PP_INDEX_LO, instr) 
                 == DT_PRE_POST_INDEX) {
            const long long simm9 = get_signed_value_from_bits(20, 12, instr);
            if (extract_bits(11, 11, instr) == DT_PRE_INDEXED)
            {
                target = (uint64_t)((long long)target + simm9);
                registers[xn_index] = (uint64_t)target;
                perform_load_or_store(rt_index, target, num_bytes, operation);
            }
            else // POST_INDEXED
            {
                perform_load_or_store(rt_index, target, num_bytes, operation);
                target = (uint64_t)((long long)target + simm9);
                registers[xn_index] = (uint64_t)target;
            }
        }
        else if (extract_bits(15, 10, instr) == DT_REG_OFFSET_MODE) // register offset
        {
            const uint32_t xm_index = extract_bits(20, 16, instr);
            target += registers[xm_index]; // might need to have a check that xm is <=30
            perform_load_or_store(rt_index, target, num_bytes, operation);
        }
    }
    else // IS_LOAD_LITERAL
    {
        long long offset = get_signed_value_from_bits(23, 5, instr) << 2;
        uint64_t target = (uint64_t)((long long)pc + offset);
        load_operation(rt_index, target, num_bytes);
    }
}
