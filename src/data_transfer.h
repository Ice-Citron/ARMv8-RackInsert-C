#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

#include "emulate.h"
#include "bit_manipulation.h"

#define UNSIGNED_IMM_OFFSET 1
#define PRE_POST_INDEXED 1
#define PRE_INDEXED 1
#define IS_32BIT_RES 1
#define IS_LOAD_OP 1
#define IS_REG_OFFSET_MODE 26
#define IS_SINGLE_DATA_TRANSFER 1

void load_operation(const uint32_t rtAddr, const uint64_t target, const int n);
void store_operation(const uint32_t rtAddr, const uint64_t target, const int n);
void perform_load_or_store(const uint32_t rtAddr, const uint64_t target, 
                           const int n, const uint32_t operation);
void single_data_transfer(const uint32_t instr);

#endif
