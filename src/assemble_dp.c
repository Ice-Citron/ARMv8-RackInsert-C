#include <assemble_dp.h>

int assemble_dp(string mnemonic, char *operands[], size_t operand_count, uint32_t word_out) {
    if (strcmp(mnemonic,"movn") == 0 || strcmp(mnemonic,"movz") == 0 || strcmp(mnemonic,"movk") == 0) {
        return assemble_wide_move(mnemonic, operands, operand_count, word_out)
    } 
    else if (strcmp(mnemonic , "madd") == 0 || strcmp(mnemonic , "msub") == 0 || strcmp(mnemonic ,"mul") == 0 || strcmp(mnemonic , "mneg") == 0) {
        return assemble_multiply(mnemonic, operands, operand_count, word_out)
    }
    else if (strcmp(mnemonic , "add") == 0 || strcmp(mnemonic ,"adds") == 0 || strcmp(mnemonic , "sub") == 0
     || strcmp(mnemonic , "subs") == 0 || strcmp(mnemonic , "cmp") == 0 || strcmp(mnemonic, "cmn") == 0 || strcmp(mnemonic , "neg") == 0 || strcmp(mnemonic , "negs") == 0) {
        
        size_t operand_2_idx = 2;

        if (strcmp(mnemonic, "cmp") == 0 || strcmp(mnemonic, "cmn") == 0 || strcmp(mnemonic, "neg") == 0 || strcmp(mnemonic, "negs") == 0) {
            operand_2_idx = 1;
        }

        if (operands[operand_2_idx] [0] == '#') {
            return assemble_dp_imm(mnemonic, operands, operand_count, word_out)
        }
        else {
            return assemble_dp_reg(mnemonic, operands, operand_count, word_out)
        }
    }
    else if (strcmp(mnemonic , "and") == 0 || strcmp(mnemonic , "bic") == 0 || strcmp(mnemonic , "orr") == 0 || strcmp(mnemonic , "orn") == 0 || strcmp(mnemonic , "eor") == 0
        || strcmp(mnemonic ,"eon") == 0 || strcmp(mnemonic ,"ands") == 0 || strcmp(mnemonic , "bics") == 0 || strcmp(mnemonic , "tst") == 0 || strcmp(mnemonic , "mov") == 0 || strcmp(mnemonic , "mvn") == 0) {
            return assemble_logical_register(mnemomic, operands, operand_count, word_out)
        }

    }

static int assemble_wide_move(string mnemonic,char *operands, size_t operand_count, uint32_t word_out) {
    uint32_t rd;
    uint32_t sf;
    uint32_t imm16;
    uint32_t shift = 0;
    uint32_t opc;

    //parse rd from operands[0] and imm16 from operands[1]

    if (operand_count == 4) {
        //parse shift from operands[3]
    }

    if (strcmp(mnemonic, "movn") == 0) {
        opc = OPC_MOVN;
    } else if (strcmp(mnemonic, "movz") == 0) {
        opc = OPC_MOVZ;
    } else {
        opc = OPC_MOVK;
    }

    uint32_t hw = shift/16u;

    *word_out = 0;

    

    return 0;

}

