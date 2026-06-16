#ifndef ASSEMBLE_FILE_H
#define ASSEMBLE_FILE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

#define ZERO_REGISTER_NUMBER 31u
#define DP_FIXED_BIT (1u << 28u)
#define DP_REG_FIXED_BITS (10u << 24u)

#define SF_SHIFT 31u
#define DP_OPC_SHIFT 29u

#define INSTR_COUNT 37
#define IMMREG_INSTR_COUNT 8
#define IMMEDIATE_CHARACTER '#'

typedef struct {
    char* instrname;
    int index;
} idx_operand_no;

typedef struct {
    const char* instrname;
    uint32_t (*handler)(char *, char* [], size_t, uint32_t);
} instr_dispatch;

uint32_t assemble_instructions(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_dp_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_dp_logical_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_multiply(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_dp_imm(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_wide_move(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

extern uint32_t ass_branch(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t ass_single_data_transfer(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t int_directive (char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

bool assemble_file(char *infile, char *outfile);

#endif // ASSEMBLE_FILE_H
