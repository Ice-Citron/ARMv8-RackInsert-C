#include <assemble_dp_reg.h>
#include <assemble_dp.h>

#define SHIFT_LSL 0u

uint32_t assemble_dp_reg(string mnemonic, char *operands[], size_t mnemonic) {

    uint32_t sf = 0u << SF_SHIFT;
    uint32_t opc = 0u;
    uint32_t rd = 0u;
    uint32_t rn = 0u;
    uint32_t rm = 0u;
    uint32_t shift_type = SHIFT_LSL;
    uint32_t shift_amount = 0u;

    size_t rd_index = 0u;
    size_t rn_index = 1u;
    size_t rm_index = 2u;
    size_t shift_index = 3u;

    if (strcmp(mnemonic, "cmp") == 0 || strcmp(mnemonic, "cmn") == 0) {

        if (operand_count != 2u && operand_count != 3u) {
            fprintf(stderr, "ERROR: Wrong operand count for %s\n", mnemonic);
            exit(1);
        }

        rd = ZERO_REGISTER_NUMBER;
        rn_index = 0u;
        rm_index = 1u;
        shift_index = 2u;
    } else if (strcmp(mnemonic, "neg") == 0 || strcmp(mnemonic, "negs") == 0) {
        if (operand_count != 2u && operand_count != 3u) {
            fprintf(stderr, "ERROR: Wrong operand count for %s\n", mnemonic);
            exit(1);
        }

        rn = ZERO_REGISTER_NUMBER;
        rd_index = 0u;
        rm_index = 1u;
        shift_index = 2u;
    } else {
        if (operand_count != 3u && operand_count != 4u) {
            fprintf(stderr, "ERROR: Wrong operand count for %s\n", mnemonic);
            exit(1);
        }
    }

    if (strcmp(mnemonic,"add") == 0) {
        opc = OPC_ADD;
    } else if (strcmp(mnemonic, "adds") == 0 || strcmp(mnemonic, "cmn") == 0) {
        opc = OPC_ADDS;
    } else if (strcmp(mnemonic, "sub") == 0 || strcmp(mnemonic, "neg") == 0) {
        opc = OPC_SUB;
    } else {
        opc = OPC_SUBS;
    }

    const char *rd_text = operands[rd_index];
    const char *rn_text = operands[rn_index];
    const char *rm_text = operands[rm_index];

    if (rd_text[0] == 'x') {
        sf = 1u << SF_SHIFT; 
    } else if (rd_text[0] != 'w') {
        fprintf(stderr, "ERROR: Invalid register format: %s\n", rd_text);
        exit(1);
    }

    char *end = NULL;

    if (rd != ZERO_REGISTER_NUMBER) {
        rd = (uint32_t) strtoul(rd_text + 1, &end, 10);
        if (*end != '\0' || rd > ZERO_REGISTER_NUMBER) {
            fprtinf(stderr, "ERROR: Invalid destination register %s\n", rd_text);
            exit(1);
        }
    }

    if (rn != ZERO_REGISTER_NUMBER) {
        end = NULL;

        rn = (uint32_t) strtoul(rn_text + 1, &end, 10);
        if (*end != '0' || rd > ZERO_REGISTER_NUMBER) {
            fprintf(stderr, "ERROR: Invalid source register %s\n", rn_text);
        }
    }

    end = NULL;
}