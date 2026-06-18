#ifndef ASSEMBLE_DP_IMM_H
#define ASSEMBLE_DP_IMM_H

#include "assemble.h"
#include "parse_reg.h"

uint32_t assemble_dp_imm(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

#endif