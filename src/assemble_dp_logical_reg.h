#ifndef ASSEMBLE_DP_LOGICAL_REG_H
#define ASSEMBLE_DP_LOGICAL_REG_H

#include "ass_helpers.h"

#define LOGICAL_NBIT_SHIFT 21u
#define LOGICAL_SHIFT_TYPE_SHIFT 22u
#define LOGICAL_RM_SHIFT 16u
#define LOGICAL_SHIFT_AMOUNT_SHIFT 10u
#define LOGICAL_RN_SHIFT 5u

#define OPC_AND 0u
#define OPC_ORR 1u
#define OPC_EOR 2u
#define OPC_ANDS 3u

#define DPLOGIC_OPC_TABLE_LENGTH 3

#define TST_OPCOUNT_NOSHIFT 2
#define TST_OPCOUNT_SHIFT 4
#define MOVMVN_OPCOUNT_NOSHIFT 2
#define MOVMVN_OPCOUNT_SHIFT 4
#define OTHER_LOGIC_OPCOUNT_NOSHIFT 3
#define OTHER_LOGIC_OPCOUNT_SHIFT 5

uint32_t assemble_dp_logical_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

#endif