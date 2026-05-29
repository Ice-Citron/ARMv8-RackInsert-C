#ifndef BRANCH_H
#define BRANCH_H

#include "bit_manipulation.h"   // for extract_bits, get_signed_value_from_bits
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

void unconditional_branch(uint32_t simm26);
void register_branch(uint32_t xn);
void conditional_branch(uint32_t simm19, uint32_t cond);
bool execute_branch(uint32_t instr);

#endif