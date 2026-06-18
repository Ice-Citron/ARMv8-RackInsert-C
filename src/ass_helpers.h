#ifndef ASSHELPERS
#define ASSHELPERS
//for helpers which run independently of the operation
#include "assemble.h"
#include "symbol_table.h"

uint32_t read_number_or_label(char *string);
uint32_t parse_reg(const char *text, uint32_t *sf);

#endif