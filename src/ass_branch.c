#include "ass_branch.h"

//instructions that we must handle
//b <literal>
//br xn
//b.cond
uint32_t ass_branch(char* mnemonic, char *operands[], size_t operand_count) {
    if (strcmp("b", mnemonic) == 0) {
        //uncon branch

    } else if (strcmp("br", mnemonic) == 0) {
        //branch register
        
    } else {
        uintcheck_mnemonic_if_cond(mnemonic);
        
    }
}