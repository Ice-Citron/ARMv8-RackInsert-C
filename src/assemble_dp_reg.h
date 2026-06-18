#ifndef ASSEMBLE_DP_REG_H
#define ASSEMBLE_DP_REG_H

#include "ass_helpers.h"
#include "assemble_file.h"

uint32_t assemble_dp_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

#endif