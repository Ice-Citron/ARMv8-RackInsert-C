#ifndef BRANCH_H
#define BRANCH_H

#include "bit_manipulation.h"
#include "emulate.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define EQ_COND 0u
#define NE_COND 1u
#define GE_COND 10u
#define LT_COND 11u
#define GT_COND 12u
#define LE_COND 13u
#define AL_COND 14u

#define BR_TYPE_UNCOND  0x05
#define BR_TYPE_REG     0x3587c0
#define BR_TYPE_COND    0x54

#define BR_UNCOND_HI    31
#define BR_UNCOND_LO    26
#define BR_REG_HI       31
#define BR_REG_LO       10
#define BR_COND_HI      31
#define BR_COND_LO      24
#define BR_SIMM26_HI    25
#define BR_SIMM26_LO    0
#define BR_SIMM19_HI    23
#define BR_SIMM19_LO    5
#define BR_XN_HI        9
#define BR_XN_LO        5
#define BR_COND_CODE_HI 3
#define BR_COND_CODE_LO 0
#define BR_OFFSET_SHIFT 2

bool execute_branch(uint32_t instr);

#endif
