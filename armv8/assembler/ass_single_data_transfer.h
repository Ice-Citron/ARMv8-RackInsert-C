#ifndef ARMV8_64_ASS_SINGLE_DATA_TRANSFER_H
#define ARMV8_64_ASS_SINGLE_DATA_TRANSFER_H

#include "bit_manipulation.h"
#include "assemble_file.h"
#include "assemble.h"
#include "ass_helpers.h"

#define MOST_SIG_BIT 31
#define L_BIT 22
#define SF_BIT 30
#define LOAD_LITERAL_BITS 24
#define LOAD_LITERAL_BITS_POS 24

#define SINGLE_DATA_TRANSFER_BITS 7
#define SINGLE_DATA_TRANSFER_BITS_POS 27
#define SIM_19_POS 5
#define SIM_19_BIT_MASK 0x7FFFF
#define UNSIGNED_IMM_OFFSET_U_BIT 24

#define UNSIGNED_IMM_OFFSET_POS 10
#define REGISTER_OFFSET_BITS 13
#define REGISTER_OFFSET_POS 11
#define REGISTER_OFFSET_BIT_POS 21
#define REGISTER_OFFSET_XM_POS 16

#define PRE_POST_INDEX_BIT_POS 10
#define PRE_INDEX_BIT_POS 11
#define PRE_POST_INDEX_SIMM9_POS 12
#define PRE_POST_INDEX_SIMM9_BITMASK 0x1FF
#define NUM_OF_REGISTERS 31

#define XN_XM_BITMASK 31
#define POS_OF_XN 5
#define DATATRANSFER_MINOPS 2
#define DATATRANSFER_MAXOPS 3
#define DTOPCOUNTS_LENGTH 1

#define LOAD_LITERAL_2OPS_COND (operand_count == 2 && strchr(operands[1], '[') != NULL && strchr(operands[1], ']') != NULL)
#define HASHTAG_ADDRESS_3OPS_LOAD_LITERAL (strchr(operands[2], '#'))
#define UNSIGNED_IMM_OFFSET_COND (strchr(operands[2], ']') != NULL && strchr(operands[2], '!') == NULL)
#define PRE_INDEX_COND (strchr(operands[2], '!') != NULL)

uint32_t ass_single_data_transfer(char* mnemonic, char *operands[],
                                  size_t operand_count, uint32_t pc);

#endif //ARMV8_64_ASS_SINGLE_DATA_TRANSFER_H
