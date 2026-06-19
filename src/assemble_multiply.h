#ifndef ASSEMBLE_MULTIPLY_H
#define ASSEMBLE_MULTIPLY_H

#include "ass_helpers.h"

#define DP_REG_FIXED_BITS (10u << 24u) 
#define MULTIPLY_M_BIT (1u<<28u)
#define MULTIPLY_OPR (8u << 21u)
#define MULTIPLY_RM_SHIFT 16u
#define MULTIPLY_X_SHIFT 15u
#define MULTIPLY_RA_SHIFT 10u
#define MULTIPLY_RN_SHIFT 5u

#define MULMNEG_OPCOUNT 3
#define OTHER_MUL_OPCOUNT 4
#define LENGTH_MULT_OPCOUNTS 2

uint32_t assemble_multiply(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

#endif