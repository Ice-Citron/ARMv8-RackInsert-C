#ifndef ASSEMBLE_DP_IMM_H
#define ASSEMBLE_DP_IMM_H

#include "ass_helpers.h"

#define DP_IMM_SH_SHIFT 22u
#define DP_IMM_IMM12_SHIFT 10u
#define DP_IMM_RN_SHIFT 5u

#define DP_IMM_ARITHMETIC_OPI (2u << 23u)
#define DP_IMM_IMM12_MASK 0xfffu
#define DP_IMM_LSL_AMOUNT 12u

uint32_t assemble_dp_imm(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

#endif