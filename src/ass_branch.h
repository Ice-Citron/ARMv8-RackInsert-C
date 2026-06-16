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
//endianness might be sus

const char *bc_strs[7] = {"eq", "ne", "ge", "lt", "gt", "le", "al"};
const int bc_values[7] = {0, 1, 10, 11, 12, 13, 14};

//check mnemonic and corresponding value
uint32_t check_mnemonic_if_cond(char* mnemonic) {
    for (int i = 0; i < NO_OF_BRANCH_CONDS; i++) {
        char inst_buffer[LENGTH_OF_COND_MNEMONIC];
        strcpy(inst_buffer, BRANCH_COND_STR_PREFIX);
        if (strncmp(mnemonic, strcat(inst_buffer, bc_strs[i]), 
                LENGTH_OF_COND_MNEMONIC)) {
            return bc_values[i];
        }
    }
    fprintf(stderr, "Invalid Cond Branch Function");
    exit(1);
}
//calculate OFFSET = label_address - caller_address
//and then sign extend to output number of bits
uint32_t calc_offset(uint32_t label_address, uint32_t caller_address, 
                     uint32_t output_bits) {
    int32_t offset_unmasked = label_address - caller_address;
    return extract_bits(output_bits - 1, 0, (uint32_t) offset_unmasked);
}

uint32_t ass_branch(char* mnemonic, char *operands[], 
                    size_t operand_count, uint32_t pc);

#endif
