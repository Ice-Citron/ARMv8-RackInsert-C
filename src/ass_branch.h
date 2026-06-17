#ifndef ASS_BRANCH_H
#define ASS_BRANCH_H

#include "assemble_file.h"
#include "bit_manipulation.h"
#include "parse_reg.h"

//uncon
#define UNCON_BRANCH_PREFIX    0x14000000
#define UNCON_BRANCH_ADDR_LENGTH       26
#define CON_BRANCH_ADDR_LENGTH         19
#define UNCON_STR_PREFIX              "b"
//register
#define REGISTER_BRANCH_PREFIX 0xD61F0000
#define XN_ADDR_SHIFT_LEFT              5
#define REG_BRANCH_STR_PREFIX        "br"
//conditional
#define CON_BRANCH_PREFIX      0x54000000
#define LENGTH_OF_COND_MNEMONIC         4
#define NO_OF_BRANCH_CONDS              7
#define CON_BRANCH_ADDR_SHIFT_LEFT      5
#define BRANCH_COND_STR_PREFIX       "b."
//misc
#define OUTPUT_BYTES_PER_LINE_LOG2 2

uint32_t ass_branch(char* mnemonic, char *operands[], 
                    size_t operand_count, uint32_t pc);

#endif
