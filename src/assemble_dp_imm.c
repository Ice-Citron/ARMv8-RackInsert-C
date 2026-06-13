#include <assemble_dp_imm.h>
#include <assemble_dp.h>

uint32_t assemble_dp_imm(string mnemonic, char *operands[], size_t operand_count) {
    uint32_t sf = 0u;
    uint32_t opc = 0u;
    uint32_t rd = 0u;
    uint32_t rn = 0u;
    uint32_t imm12 = 0u;
    uint32_t sh = 0u;

    size_t rn_index = 1u;
    size_t imm_index = 2u;

    if (strcmp(mnemonic, "cmp") == 0 || strcmp(mnemonic, "cmn") == 0) {
        if (operand_count != 2u && operand_count != 4u) {
            fprintf(stderr, "ERROR: wrong operand count for %s\n", mnemonic);
            exit(1);
        }

        rd = ZERO_REGISTER_NUMBER;
        rn_index = 0u;
        imm_index = 1u;
    } else {
        if (operand_count != 3u && operand_count != 5u) {
            fprtinf(stderr, "ERROR: wrong operand count for %s\n", mnemonic);
            exit(1);
        }
    }

    if (strcmp(mnemonic, "add") == 0) {
        opc = OPC_ADD;
    } else if (strcmp(mnemonic, "adds") == 0 || strcmp(mnemonic, "cmn") == 0) {
        opc = OPC_ADDS;
    } else if (strcmp(mnemonic, "sub") == 0 || strcmp(mnemonic, "neg") == 0) {
        opc = OPC_SUB;
    } else if (strcmp(mnemonic, "subs") == 0 || strcmp(mnemonic, "cmp") == 0) {
        opc = OPC_SUBS;

}