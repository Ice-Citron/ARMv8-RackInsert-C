#ifndef ASSEMBLE_DP_REG_H
#define ASSEMBLE_DP_REG_H

#include "ass_helpers.h"
#include "assemble_file.h"

#define DP_REG_ARITHMETIC_OPR (8u << 21u)
#define DP_REG_RM_SHIFT 16u
#define DP_REG_SHIFTAMOUNT_SHIFT 10u
#define DP_REG_RN_SHIFT 5u
#define SHIFT_TYPE_SHIFT 22u

uint32_t assemble_dp_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

#endif