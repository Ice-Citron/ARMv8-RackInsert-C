//this is meant to unify register parsing methods 
//across the codebase
#ifndef PARSEREG
#define PARSEREG
#include "assemble_file.h"
#include <stdint.h>

#define REG_X_SF 1
#define REG_W_SF 0

uint32_t parse_reg(const char *text, uint32_t *sf);

#endif