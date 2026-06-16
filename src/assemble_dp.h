#ifndef ASSEMBLE_DP_H
#define ASSEMBLE_DP_H

#include "assemble.h"

#define ZERO_REGISTER_NUMBER 31u
#define DP_FIXED_BIT (1u << 28u)
#define DP_REG_FIXED_BITS (10u << 24u)

#define SF_SHIFT 31u
#define DP_OPC_SHIFT 29u

#define INSTR_COUNT 25
#define IMMREG_INSTR_COUNT 8
#define IMMEDIATE_CHARACTER '#'

typedef struct {
    char* instrname;
    int index;
} idx_operand_no;

uint32_t assemble_dp(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_dp_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_dp_logical_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_multiply(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_dp_imm(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_wide_move(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

#endif