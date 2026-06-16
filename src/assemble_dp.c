#include "assemble_dp.h"

#define INSTR_COUNT 25
#define IMMREG_INSTR_COUNT 8
#define IMMEDIATE_CHARACTER '#'

//decides index where we check whether 
//function is register or immediate
static idx_operand_no immreg_instrs[] = {
    {"add", 2},
    {"adds", 2},
    {"sub", 2},
    {"subs", 2},
    {"cmp", 1},
    {"cmn", 1},
    {"neg", 1},
    {"negs", 1},
};

//function handling decision whether immediate or register for dpimm and dpreg instrs
static uint32_t decide_imm_or_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {
    size_t operand_2_idx;
    for (int i = 0; i < IMMREG_INSTR_COUNT; i++) {
        if (strcmp(mnemonic, immreg_instrs[i].instrname) == 0) {
            operand_2_idx = immreg_instrs[i].index;
        }
        if (operands[operand_2_idx] [0] == IMMEDIATE_CHARACTER) {
            return assemble_dp_imm(mnemonic, operands, operand_count, pc);
        }
        else {
            return assemble_dp_reg(mnemonic, operands, operand_count, pc);
        }
    }
    fprintf(stderr, "function name doesnt exist");
    exit(1);
}

//dispatch table for app dp instructions
static instr_dispatch dp_instrs[] = {
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
    {"mvn", assemble_dp_logical_reg},
    //8 decide imm or reg
    {"add", decide_imm_or_reg},
    {"adds", decide_imm_or_reg},
    {"sub", decide_imm_or_reg},
    {"subs", decide_imm_or_reg},
    {"cmp", decide_imm_or_reg},
    {"cmn", decide_imm_or_reg},
    {"neg", decide_imm_or_reg},
    {"negs", decide_imm_or_reg},
};

//main dp assembling function
uint32_t assemble_dp(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {
    for (int i = 0; i < INSTR_COUNT; i++) {
        if (strcmp(mnemonic, dp_instrs[i].instrname) == 0) {
            return dp_instrs[i].handler(mnemonic, operands, operand_count, pc);
        }
    }
    fprintf(stderr, "function name doesnt exist");
    exit(1);
}

