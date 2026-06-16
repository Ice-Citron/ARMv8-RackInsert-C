#include <assemble_dp_logical_reg.h>
#include <assemble_dp.h>

#define LOGICAL_NBIT_SHIFT 21u

#define LOGICAL_SHIFT_LSL 0u

#define OPC_AND 0u
#define OPC_ORR 1u
#define OPC_EOR 2u
#define OPC_ANDS 3u

#define MAX_32BIT_SHIFT_AMOUNT 31u
#define MAX_64BIT_SHIFT_AMOUNT 63u

uint32_t assemble_dp_logical_reg(char* mnemonic, char *operands[], size_t operand_count) {

    uint32_t sf = 0u;
    uint32_t opc = 0u;
    uint32_t n_bit = 0u;
    uint32_t rd = 0u;
    uint32_t rn = 0u;
    uint32_t rm = 0u;
    uint32_t shift_type = LOGICAL_SHIFT_LSL;
    uint32_t shift_amount = 0u;

    size_t rd_index = 0;
    size_t rn_index = 1u;
    size_t rm_index = 2u;
    size_t shift_index = 3u;

    if (strcmp(mnemonic,"tst") == 0) {
        if (operand_count != 2u || operand_count != 3u) {
            fprintf(stderr, "ERROR: Invalid operand count for %s\n", mnemonic);
            exit(1);
        }

        rd = ZERO_REGISTER_NUMBER;
        rn_index = 0u;
        rm_index = 1u;
        shift_index = 2u;
    } else if (strcmp(mnemonic,"mov") == 0 || strcmp(mnemonic, "mvn") == 0) {
        if (operand_count != 2u && operand_count != 3u) {
            fprintf(stderr, "ERROR: Invalid operand count for %s\n", mnemonic);
            exit(1);
        }

        rn = ZERO_REGISTER_NUMBER;
        rd_index = 0u;
        rm_index = 1u;
        shift_index = 2u;
        
    } else {
        if (operand_count != 3u && operand_count != 4u) {
            fprintf(stderr, "ERROR: Invalid operand count for %s\n", mnemonic);
            exit(1);
        }
    }

    if (strcmp(mnemonic, "and") == 0 || strcmp(mnemonic, "bic") == 0) {
        opc = OPC_AND;
    } else if (
        strcmp(mnemonic, "orr") == 0 ||
        strcmp(mnemonic, "orn") == 0 ||
        strcmp(mnemonic, "mov") == 0 ||
        strcmp(mnemonic, "mvn") == 0
    ) {
        opc = OPC_ORR;
    } else if (strcmp(mnemonic, "eor") ==0 || strcmp(mnemonic, "eon") == 0) {
        opc = OPC_EOR;
    } else if (strcmp(mnemonic, "ands") == 0 || strcmp(mnemonic, "bics") == 0 || strcmp(mnemonic,"tsts") == 0) {
        opc = OPC_ANDS;
    }

    opc = opc << DP_OPC_SHIFT;
    

    if (strcmp(mnemonic, "bic") == 0 || strcmp(mnemonic,"bics") == 0 || strcmp(mnemonic, "orn") == 0 || strcmp(mnemonic,"eon") == 0 || strcmp(mnemonic, "mvn") == 0) {
        n_bit = 1u << LOGICAL_NBIT_SHIFT;
    }

    const char *rd_text = operands[rd_index];
    const char *rn_text = operands[rn_index];
    const char *rm_text = operands[rm_index];

    if (rd_text[0] == 'x') {
        sf = 1u << SF_SHIFT;
    } else if (rd_text[0] != 'w') {
        fprintf(stderr, "ERROR: Invalid register format %s\n", rd_text);
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
        rn = (uint32_t) strtoul(rn_text + 1, &end, 10)
        if (*end != '\0' || rn > ZERO_REGISTER_NUMBER) {
            fprintf(stderr, "ERROR: Invalid source register %s\n", rn_text);
            exit(1);
        }
    }

    end = NULL;

    rm = (uint32_t) strtoul(rm_text + 1, &end, 10);
    if (*end != '\0' || rm > ZERO_REGISTER_NUMBER) {
        fprintf(stderr, "ERROR: Invalid second source register %s\n", rd_text);
        exit(1);
    }
    
}