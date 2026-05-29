#ifndef BRANCH_H
#define BRANCH_H

#include "bit_manipulation.h"   // for extract_bits, get_signed_value_from_bits
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

bool unconditional_branch(long long simm26);
bool register_branch(uint32_t xn);
bool conditional_branch(long long simm19, uint32_t cond);
bool execute_branch(uint32_t instr);

#endif
