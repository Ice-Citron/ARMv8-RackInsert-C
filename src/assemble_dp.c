#include <assemble_dp.h>

#define INSTR_COUNT 17
//set for further expansion 

instr_dispatch dp_instrs[] = {
    //3 assemble wide move
    {"movn", assemble_wide_move},
    {"movz", assemble_wide_move},
    {"movk", assemble_wide_move},
    //4 assemble multiply
    {"madd", assemble_multiply},
    {"msub", assemble_multiply},
    {"mul", assemble_multiply},
    {"mneg", assemble_multiply},
    //10 logical reg
    {"and", assemble_dp_logical_reg},
    {"bic", assemble_dp_logical_reg},
    {"orr", assemble_dp_logical_reg},
    {"eor", assemble_dp_logical_reg},
    {"eon", assemble_dp_logical_reg},
    {"ands", assemble_dp_logical_reg},
    {"bics", assemble_dp_logical_reg},
    {"tst", assemble_dp_logical_reg},
    {"mov", assemble_dp_logical_reg},
    {"mvn", assemble_dp_logical_reg}
};

static uint32_t decide_imm_or_reg(char* mnemonic, char *operands[], size_t operand_count) {
    
}

//make dispatch table just for dp
uint32_t assemble_dp(char* mnemonic, char *operands[], size_t operand_count) {
    if (strcmp(mnemonic , "add") == 0 || strcmp(mnemonic ,"adds") == 0 || strcmp(mnemonic , "sub") == 0
     || strcmp(mnemonic , "subs") == 0 || strcmp(mnemonic , "cmp") == 0 || strcmp(mnemonic, "cmn") == 0 || strcmp(mnemonic , "neg") == 0 || strcmp(mnemonic , "negs") == 0) {
        //we are gonna make a seperate dispatch for what is the operand_2_idx here
        //exclude these 2 from the main dispatch table
        size_t operand_2_idx = 2;

        if (strcmp(mnemonic, "cmp") == 0 || strcmp(mnemonic, "cmn") == 0 || strcmp(mnemonic, "neg") == 0 || strcmp(mnemonic, "negs") == 0) {
            operand_2_idx = 1;
        }
        if (operands[operand_2_idx] [0] == '#') {
            return assemble_dp_imm(mnemonic, operands, operand_count);
        }
        else {
            return assemble_dp_reg(mnemonic, operands, operand_count);
        }
    }
    for (int i = 0; i < INSTR_COUNT; i++) {
        if (strcmp(mnemonic, dp_instrs[i].instrname) == 0) {
            dp_instrs[i].handler(mnemonic, operands, operand_count);
        }
    }

}

