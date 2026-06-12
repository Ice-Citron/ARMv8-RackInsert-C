#include "data_transfer.h"
#include "bit_manipulation.h"
#include <inttypes.h>

#define DT_UINT_IMM_OFFSET 1
#define DT_PRE_POST_INDEX  1
#define DT_PRE_IDX         1
#define DT_32BIT_MODE      0
#define DT_LOAD_OP         1
#define DT_SINGLE_MODE     1
#define DT_REG_OFFSET_MODE 26

#define DT_RT_HI     4
#define DT_RT_LO     0
#define DT_SIZE_HI   30
#define DT_SIZE_LO   30
#define DT_XN_HI     9
#define DT_XN_LO     5
#define DT_OP_HI     22
#define DT_OP_LO     22
#define DT_SINGLE_HI 31
#define DT_SINGLE_LO 31
#define DT_UINT_HI   24
#define DT_UINT_LO   24
#define DT_IMM12_HI  21
#define DT_IMM12_LO  10
#define DT_SIMM9_HI  20
#define DT_SIMM9_LO  12
#define DT_PINDEX_HI 11
#define DT_PINDEX_LO 11
#define DT_PRE_POST_HI 10
#define DT_PRE_POST_LO 10
#define DT_OFFSET_HI 15
#define DT_OFFSET_LO 10
#define DT_XM_HI     20
#define DT_XM_LO     16
#define DT_LOAD_HI   23
#define DT_LOAD_LO   5

/*
 * (Helper) Loads n bytes from memory into a register using little-endian order.
 * Example: memory bytes 78 56 34 12 load as 0x12345678.
 */
static void load_operation(const uint32_t rt_index, const uint64_t target, 
                           const int n) {
    registers[rt_index] = 0;
    for (int i = 0; i < n; i++) {
        uint64_t mem_addr = target + (uint64_t)i;    
        registers[rt_index] |= ((uint64_t)memory[mem_addr] << (i * 8)); 
    }
}

/*
 * (Helper) Stores n bytes from a register into memory using little-endian order
 * Example: value 0x1234 stores byte 0x34 then byte 0x12.
 */
static void store_operation(const uint32_t rt_index, const uint64_t target, 
                            const int n) {
    for (int i = 0; i < n; i++) { 
        uint64_t mem_addr = target + (uint64_t)i;
        memory[mem_addr] = (uint8_t)(registers[rt_index] >> (i * 8)) & 0xff; 
    }
}

/*
 * (Helper) Performs either a load or store depending on the operation bit.
 * Example: DT_LOAD_OP calls load_operation; otherwise store_operation.
 */
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

/*
 * (Helper) Handles unsigned immediate offset addressing.
 * Example: LDR X0, [X1, #8] loads from address X1 + 8.
 */
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

/*
 * (Helper) Handles pre-indexed and post-indexed addressing.
 * Example: pre-index updates the base before transfer; post-index updates it 
 * after.
 */
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

/*
 * (Helper) Handles register offset addressing.
 * Example: STR X0, [X1, X2] stores to address X1 + X2.
 */
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

/*
 * (Helper) Handles load literal instructions using a PC-relative address.
 * Example: LDR X0, label loads from PC plus the signed literal offset.
 */
static void handle_load_literal(const uint32_t instr, const uint32_t rt_index, 
                         int num_bytes) {
    long long offset = get_signed_value(DT_LOAD_HI, DT_LOAD_LO, instr) << 2;
    uint64_t target = (uint64_t)((long long)pc + offset);
    load_operation(rt_index, target, num_bytes);
}

/*
 * Decodes and executes load/store instructions.
 * Example: dispatches unsigned offset, pre/post-indexed, register offset
 * and load literal addressing modes.
 */
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
