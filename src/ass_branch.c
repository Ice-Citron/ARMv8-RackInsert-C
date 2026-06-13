#include "ass_branch.h"

//instructions that we must handle
//b <literal>
//br xn
//b.cond
uint32_t ass_branch(char* mnemonic, char *operands[], 
                    size_t operand_count, uint32_t pc) {
    //check number of operands
    if (operand_count != 2) {
        fprintf(stderr, "wrong number of operands");
        exit(1);
    }
    //spec says we can assume that literal will be label
    uint32_t target_memory = find_address_from_sym_table(operands[1]);
    if (strcmp(UNCON_STR_PREFIX, mnemonic) == 0) {
        //uncon branch
        return UNCON_BRANCH_PREFIX | (calc_offset(target_memory, pc, UNCON_BRANCH_ADDR_LENGTH))

    } else if (strcmp(REG_BRANCH_STR_PREFIX, mnemonic) == 0) {
        //branch register
        return REGISTER_BRANCH_PREFIX | (atoi(operands[1]) << REG_INDEX_SHIFT_LEFT)
    } else {
        //branch conditional
        uint32_t opcode = check_mnemonic_if_cond(mnemonic);
        
        
    }
}