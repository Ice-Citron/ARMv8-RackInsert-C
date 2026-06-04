#include <assemble_dp.h>

int assemble_dp(string mnemonic, char *operands[]) {
    if (mnemonic == "movn" || mnemonic == "movz" || mnemonic == "movk") {
        return assemble_wide_move(mnemonic, *operands)
    } 
    else if (mnemonic == "madd" || mnemonic == "msub" || mnemonic == "mul" || mnemonic == "mneg") {
        return assemble_multiply(mnemonic, *operands)
    }
    else if (mnemonic == "add" || mnemonic == "adds" || mnemonic == "sub"
     || mnemonic == "subs" || mnemonic == "cmp" || mnemonic == "cmn" || mnemonic == "neg" || mnemonic == "negs") {
        arithmetic(mnemonic, *operands)
        if (operands[2][0] == "#") {
            return assemble_dp_imm(mnemonic, *operands[])
        }
        else {
            return assemble_dp_reg(mnemonic, *operands[])
        }
    }
    else if (mnemonic == "and" || mnemonic == "bic" || mnemonic == "orr" || mnemonic == "orn" || mnemonic == "eor" 
        || mnemonic == "eon" || mnemonic == "ands" || mnemonic == "bics" || mnemonic == "tst" || mnemonic == "mov" || mnemonic == "mvn") {
            return assemble_logical_register()
        }

}