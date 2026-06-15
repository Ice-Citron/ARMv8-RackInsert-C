#ifndef ASSEMBLE_DP_H
#define ASSEMBLE_DP_H

#include <assemble.h>

#define ZERO_REGISTER_NUMBER 31u
#define DP_FIXED_BIT (1u << 28u)

#define SF_SHIFT 31u
#define DP_OPC_SHIFT 29u

uint32_t assemble_dp(char* mnemonic, char *operands[], size_t operand_count);

#endif