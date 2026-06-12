#ifndef ASSEMBLE_DP_H
#define ASSEMBLE_DP_H

#include <stdlib.h>
#include <stddef.h>

#define ZERO_REGISTER_NUMBER 31u

#define WIDE_MOVE_FIXED_BIT 1u << 28u
#define WIDE_MOVE_OPI 5u << 23u

#define SF_SHIFT 31u
#define DP_OPC_SHIFT 29u
#define WIDE_MOVE_HW_SHIFT 21u
#define WIDE_MOVE_IMM16_SHIFT 5u
#define WIDE_MOVE_SHIFT_UNIT 16u
#define WIDE_MOVE_MAX_SHIFT 48u
#define WIDE_MOVE_IMM16_MASK 0xffffu

#define OPC_MOVN 0u
#define OPC_MOVZ 2u
#define OPC_MOVK 3u


int assemble_dp(string mnemonic, char *operands[]);

#endif