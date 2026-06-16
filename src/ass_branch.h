#ifndef ASS_BRANCH_H
#define ASS_BRANCH_H

#include "assemble_file.h"
#include "bit_manipulation.h"

//uncon
#define UNCON_BRANCH_PREFIX     335544320
#define UNCON_BRANCH_ADDR_LENGTH       26
#define CON_BRANCH_ADDR_LENGTH         19
#define UNCON_STR_PREFIX              "b"
//register
#define REGISTER_BRANCH_PREFIX 3592355840
#define XN_ADDR_SHIFT_LEFT              5
#define REG_BRANCH_STR_PREFIX        "br"
//conditional
#define CON_BRANCH_PREFIX      1409286144
#define LENGTH_OF_COND_MNEMONIC         4
#define NO_OF_BRANCH_CONDS              7
#define CON_BRANCH_ADDR_SHIFT_LEFT      5
#define BRANCH_COND_STR_PREFIX       "b."

uint32_t ass_branch(char* mnemonic, char *operands[], 
                    size_t operand_count, uint32_t pc);

#endif
