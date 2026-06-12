#include "assemble_file.h"


#define UNCON_BRANCH_PREFIX     335544320
#define REGISTER_BRANCH_PREFIX 3592355840
#define CON_BRANCH_PREFIX      1409286144
#define XN_ADDR_SHIFT_LEFT              5
#define CON_BRANCH_ADDR_SHIFT_LEFT      5
#define UNCON_BRANCH_ADDR_LENGTH       26
#define CON_BRANCH_ADDR_LENGTH         19
//endianness might be sus
#define LENGTH_OF_COND_MNEMONIC         4
#define NO_OF_BRANCH_CONDS              7
#define BRANCH_COND_STR_PREFIX       "b."

const char *bc_strs[7] = {"eq", "ne", "ge", "lt", "gt", "le", "al"};
const int bc_values[7] = {0, 1, 10, 11, 12, 13, 14};

//check mnemonic and corresponding value
int check_mnemonic_if_cond(char* mnemonic) {
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
