#include "data_transfer.h"
#include <inttypes.h>

static void load_operation(const uint32_t rt_index, const uint64_t target, 
                           const int n) {
    registers[rt_index] = 0;
    for (int i = 0; i < n; i++) {
        uint64_t mem_addr = target + (uint64_t)i;    
        registers[rt_index] |= ((uint64_t)memory[mem_addr] << (i * 8)); 
    }
}

static void store_operation(const uint32_t rt_index, const uint64_t target, 
                            const int n) {
    for (int i = 0; i < n; i++) { 
        uint64_t mem_addr = target + (uint64_t)i;
        memory[mem_addr] = (uint8_t)(registers[rt_index] >> (i * 8)) & 0xff; 
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

static void handle_unsigned_offset(const uint32_t instr, uint64_t target, 
                                   const uint32_t rt_index, const int num_bytes, 
                                   const uint32_t operation, 
                                   const uint32_t size) {
    const uint32_t imm12 = extract_bits(DT_IMM12_HI, DT_IMM12_LO, instr);
    if (size == DT_32BIT_MODE) {
        target += (imm12 << 2);
    } else {
        target += (imm12 << 3);
    }
    perform_load_or_store(rt_index, target, num_bytes, operation);
}

static void handle_pre_post_indexed(const uint32_t instr, uint64_t target, 
                                    const uint32_t xn_index, 
                                    const uint32_t rt_index, int num_bytes, 
                                    const uint32_t operation) {
    const long long simm9 = get_signed_value(DT_SIMM9_HI, DT_SIMM9_LO, instr);
    if (extract_bits(DT_PINDEX_HI, DT_PINDEX_LO, instr) == DT_PRE_IDX) {
        target = (uint64_t)((long long)target + simm9);
        registers[xn_index] = (uint64_t)target;
        perform_load_or_store(rt_index, target, num_bytes, operation);
    } else { // POST_INDEXED
        perform_load_or_store(rt_index, target, num_bytes, operation);
        target = (uint64_t)((long long)target + simm9);
        registers[xn_index] = (uint64_t)target;
    }
}

static void handle_register_offset(const uint32_t instr, uint64_t target, 
                            const uint32_t rt_index, int num_bytes, 
                            const uint32_t operation) {
    const uint32_t xm_index = extract_bits(DT_XM_HI, DT_XM_LO, instr);
    if (xm_index > 30){
        fprintf(stderr, "ERROR: There's only 31 registers, Register %"
                        PRIu32" doesn not exist\n", xm_index);
        exit(EXIT_FAILURE);
    }
    target += registers[xm_index];
    perform_load_or_store(rt_index, target, num_bytes, operation);
}

static void handle_load_literal(const uint32_t instr, const uint32_t rt_index, 
                         int num_bytes) {
    long long offset = get_signed_value(DT_LOAD_HI, DT_LOAD_LO, instr) << 2;
    uint64_t target = (uint64_t)((long long)pc + offset);
    load_operation(rt_index, target, num_bytes);
}

void single_data_transfer(const uint32_t instr) {
    const uint32_t rt_index = extract_bits(DT_RT_HI, DT_RT_LO, instr);
    const uint32_t size     = extract_bits(DT_SIZE_HI, DT_SIZE_LO, instr);
    int num_bytes = (size == DT_32BIT_MODE) ? 4 : 8;

    if (extract_bits(DT_SINGLE_HI, DT_SINGLE_LO, instr) == DT_SINGLE_MODE) {
        const uint32_t xn_index  = extract_bits(DT_XN_HI, DT_XN_LO, instr);
        const uint32_t operation = extract_bits(DT_OP_HI, DT_OP_LO, instr);
        uint64_t target = registers[xn_index];
        if (extract_bits(DT_UINT_HI, DT_UINT_LO, instr) == DT_UINT_IMM_OFFSET) {
            handle_unsigned_offset(instr, target, rt_index, num_bytes, 
                                   operation, size);
        } else if (extract_bits(DT_PRE_POST_HI, DT_PRE_POST_LO, instr) 
                   == DT_PRE_POST_INDEX) {
            handle_pre_post_indexed(instr, target, xn_index, rt_index, 
                                    num_bytes, operation);
        } else if (extract_bits(DT_OFFSET_HI, DT_OFFSET_LO, instr) 
                   == DT_REG_OFFSET_MODE) {
            handle_register_offset(instr, target, rt_index, num_bytes, 
                                   operation);
        } else {
            fprintf(stderr, "ERROR: Unrecognsied single data transfer "
                            "addressing mode.\n");
            exit(EXIT_FAILURE);
        }
    } else { 
        handle_load_literal(instr, rt_index, num_bytes);
    }
}
