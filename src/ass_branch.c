#include "ass_branch.h"

#define BRANCH_NUMBEROF_OPERANDS 1

//endianness might be sus

const char *bc_strs[7] = {"eq", "ne", "ge", "lt", "gt", "le", "al"};
const int bc_values[7] = {0, 1, 10, 11, 12, 13, 14};

//check mnemonic and corresponding value
static uint32_t check_mnemonic_if_cond(char* mnemonic) {
    for (int i = 0; i < NO_OF_BRANCH_CONDS; i++) {
        char inst_buffer[LENGTH_OF_COND_MNEMONIC];
        strcpy(inst_buffer, BRANCH_COND_STR_PREFIX);
        if (strncmp(mnemonic, strcat(inst_buffer, bc_strs[i]), 
                LENGTH_OF_COND_MNEMONIC) == 0) {
            return bc_values[i];
        }
    }
    fprintf(stderr, "Invalid Cond Branch Function");
    exit(1);
}
//calculate OFFSET = label_address - caller_address
//and then sign extend to output number of bits
static uint32_t calc_offset(uint32_t label_address, uint32_t caller_address, 
                     uint32_t output_bits) {
    int32_t offset_unmasked = label_address - caller_address;
    return extract_bits(output_bits - 1, 0, (uint32_t) offset_unmasked);
}

//instructions that we must handle
//b <literal>
//br xn
//b.cond
uint32_t ass_branch(char* mnemonic, char *operands[], 
                    size_t operand_count, uint32_t pc) {
    //check number of operands
    if (operand_count != BRANCH_NUMBEROF_OPERANDS) {
        fprintf(stderr, "wrong number of operands");
        exit(1);
    }
    //i know it should be a bitwise op but lets pass tests first
    //target can be label or value
    uint32_t target_memory = read_number_or_label(operands[0]);
    if (strcmp(UNCON_STR_PREFIX, mnemonic) == 0) {
        //uncon branch
        return UNCON_BRANCH_PREFIX + (calc_offset(target_memory, pc, UNCON_BRANCH_ADDR_LENGTH));

    } else if (strcmp(REG_BRANCH_STR_PREFIX, mnemonic) == 0) {
        //branch register - reads from the second character (e.g. register is x0)
        return REGISTER_BRANCH_PREFIX + (atoi(operands[0] + 1) << XN_ADDR_SHIFT_LEFT);
    } else {
        //branch conditional
        uint32_t opcode = check_mnemonic_if_cond(mnemonic);
        return CON_BRANCH_PREFIX  +
            (calc_offset(target_memory, pc, CON_BRANCH_ADDR_LENGTH) << CON_BRANCH_ADDR_SHIFT_LEFT) +
            opcode;
        
        
    }
}