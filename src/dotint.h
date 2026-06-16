#ifndef DOT_INT_H
#define DOT_INT_H

#include "assemble.h"
#define HEX_TYPE_SIGNATURE "0x"
#define LEN_HEX_TYPE_SIGNATURE 2
#define HEX_BASE 16
#define DECI_BASE 10

uint32_t int_directive (char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

#endif
