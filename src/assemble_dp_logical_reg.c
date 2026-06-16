#include <assemble_dp_logical_reg.h>
#include <assemble_dp.h>

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
    
    
}