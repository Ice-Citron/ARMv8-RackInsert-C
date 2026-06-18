#ifndef ASSEMBLE_DP_LOGICAL_REG_H
#define ASSEMBLE_DP_LOGICAL_REG_H

#include "assemble.h"
#include "parse_reg.h"
#include "ass_helpers.h"

uint32_t assemble_dp_logical_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

#endif