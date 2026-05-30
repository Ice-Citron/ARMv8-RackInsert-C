#ifndef EMULATOR_LOADER_H
#define EMULATOR_LOADER_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

#define OP0_DP_IMM_1000     0x08
#define OP0_DP_IMM_1001     0x09
#define OP0_DP_REG_0101     0x05
#define OP0_DP_REG_1101     0x0d
#define OP0_LOAD_STORE_0100 0x04
#define OP0_LOAD_STORE_0110 0x06
#define OP0_LOAD_STORE_1100 0x0c
#define OP0_LOAD_STORE_1110 0x0e
#define OP0_BRANCH_1010     0x0a
#define OP0_BRANCH_1011     0x0b

#define OP0_HI 28
#define OP0_LO 25

#define INSTRUCTION_BYTES 4
#define WORD_BYTES        4


bool load_program(const char *path, size_t *bytes_loaded);
uint32_t fetch_u32_le(uint64_t address);
void run_emulator(void);
void write_final_state(FILE *out);

#endif
