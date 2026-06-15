#include <assemble_dp_imm.h>
#include <assemble_dp.h>


uint32_t assemble_dp_imm(string mnemonic, char *operands[], size_t operand_count) {
    uint32_t sf = 0u << SF_SHIFT;
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

    if (operands[0][0] == 'x') {
        sf = 1u << SF_SHIFT;
    } else if (operands[0][0] != 'w') {
        fprintf(stderr, "ERROR: invalid register %s\n", operands[0]);
        exit(1);
    }

    char *end = NULL;

    if (rd != ZERO_REGISTER_NUMBER) {
        rd = (uint32_t) strtoul(operands[0] + 1, &end, 10);
        if (*end != '\0' || rd > ZERO_REGISTER_NUMBER) {
            fprintf(stderr, "ERROR: Invalid destination register %s\n", operands[0]);
            exit(1);
        }
    }

    if (rn != ZERO_REGISTER_NUMBER) {
        end = NULL;
        rn = (uint32_t) strtoul(operands[rn_index] + 1, &end, 10);
        if (*end != '\0' || rn > ZERO_REGISTER_NUMBER) {
            fprintf(stderr, "ERROR: Invalid source register %s\n", operands[rn_index])
            exit(1);
        }
    }

    if (operands[imm_index][0] != '#') {
        fprtinf(stderr, "ERROR: Invalid immediate addressing format, must begin with # %s\n", operands[imm_index])
    }

    end = NULL;
    unsigned long parsed_imm = strtoul(operands[imm_index] + 1, &end, 10);

    if (*end != '\0')




}