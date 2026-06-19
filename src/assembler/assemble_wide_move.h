#ifndef ASSEMBLE_WIDE_MOVE_H
#define ASSEMBLE_WIDE_MOVE_H

#include "ass_helpers.h"

#define WIDE_MOVE_OPI (5u << 23u)
#define WIDE_MOVE_HW_SHIFT 21u
#define WIDE_MOVE_IMM16_SHIFT 5u
#define WIDE_MOVE_SHIFT_UNIT 16u
#define WIDE_MOVE_MAX_SHIFT 48u
#define WIDE_MOVE_IMM16_MASK 0xffffu

#define OPC_MOVN 0u
#define OPC_MOVZ 2u
#define OPC_MOVK 3u

#define WM_NOSHIFT_OPCOUNT 2
#define WM_SHIFT_OPCOUNT 4
#define LENGTH_WM_OPCOUNTS 1

uint32_t assemble_wide_move(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

#endif

