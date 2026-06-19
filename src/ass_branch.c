#include "ass_branch.h"

#define BRANCH_NUMBEROF_OPERANDS 1
#define NUMBER_OF_LOGICAL_OPS 7
//endianness might be sus

const char *bc_strs[NUMBER_OF_LOGICAL_OPS] = {"eq", "ne", "ge", "lt", "gt", "le", "al"};
const int bc_values[NUMBER_OF_LOGICAL_OPS] = {0, 1, 10, 11, 12, 13, 14};

//check mnemonic and return the corresponding value (opcode)
static uint32_t check_mnemonic_if_cond(char* mnemonic) {
    for (int i = 0; i < NO_OF_BRANCH_CONDS; i++) {
        char inst_buffer[LENGTH_OF_COND_MNEMONIC];
        strcpy(inst_buffer, BRANCH_COND_STR_PREFIX);
        if (strncmp(mnemonic, strcat(inst_buffer, bc_strs[i]), 
                LENGTH_OF_COND_MNEMONIC) == 0) {
            return bc_values[i];
        }
    }
    print_error_and_exit("INVALID COND BRANCH FUNCTION");
    exit(1);
}

//calculate OFFSET = label_address - caller_address
//and then sign extend to output number of bits
static uint32_t calc_offset(uint32_t label_address, uint32_t caller_address, 
                     uint32_t output_bits) {
    int32_t offset_unmasked = (label_address - caller_address) >> OUTPUT_BYTES_PER_LINE_LOG2;
    return extract_bits(output_bits - 1, 0, (uint32_t) offset_unmasked);
}

static opcount_checker branch_opcounts[] = {
    {"branches", 1, 1},
};

//instructions that we must handle
//b <literal>
//br xn
//b.cond
uint32_t ass_branch(char* mnemonic, char *operands[], 
                    size_t operand_count, uint32_t pc) {
    //check number of operands
    check_opcount(branch_opcounts, LENGTH_BRANCH_OPCOUNTS, "branches", operand_count);
    //target can be label or value
    uint32_t target_memory;
    if (strcmp(REG_BRANCH_STR_PREFIX, mnemonic) == 0) {
        uint32_t dummy_sf;
        //branch register - reads from the second character (e.g. register is x0)
        return REGISTER_BRANCH_PREFIX | (parse_reg(operands[0], &dummy_sf) << XN_ADDR_SHIFT_LEFT);
    } else if (strcmp(UNCON_STR_PREFIX, mnemonic) == 0) {
        //uncon branch
        //printf("uncon: target memory %d", target_memory);
        target_memory = read_number_or_label(operands[0]);
        return UNCON_BRANCH_PREFIX | (calc_offset(target_memory, pc, UNCON_BRANCH_ADDR_LENGTH));
    } else {
        //branch conditional
        target_memory = read_number_or_label(operands[0]);
        uint32_t opcode = check_mnemonic_if_cond(mnemonic);
        return CON_BRANCH_PREFIX |
            (calc_offset(target_memory, pc, CON_BRANCH_ADDR_LENGTH) << CON_BRANCH_ADDR_SHIFT_LEFT) |
            opcode;    
    }
}