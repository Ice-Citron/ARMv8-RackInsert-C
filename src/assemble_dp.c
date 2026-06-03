#include <assemble_dp.h>

int assemble_dp(string mnemonic, char *operands[]) {
    if (mnemonic == "movn" || mnemonic == "movz" || mnemonic == "movk") {
        wide_move(mnemonic, *operands)
    } 
    else if (mnemonic == "madd" || mnemonic == "msub" || mnemonic == "mul" || mnemonic == "mneg") {
        multiply(mnemonic, *operands)
    }
}