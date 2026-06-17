#include "assemble_dp_logical_reg.h"
#include "assemble_file.h"

#define LOGICAL_NBIT_SHIFT 21u
#define LOGICAL_SHIFT_TYPE_SHIFT 22u
#define LOGICAL_RM_SHIFT 16u
#define LOGICAL_SHIFT_AMOUNT_SHIFT 10u
#define LOGICAL_RN_SHIFT 5u

#define LOGICAL_SHIFT_LSL 0u
#define LOGICAL_SHIFT_LSR 1u
#define LOGICAL_SHIFT_ASR 2u
#define LOGICAL_SHIFT_ROR 3u

#define OPC_AND 0u
#define OPC_ORR 1u
#define OPC_EOR 2u
#define OPC_ANDS 3u

#define MAX_32BIT_SHIFT_AMOUNT 31u
#define MAX_64BIT_SHIFT_AMOUNT 63u

uint32_t assemble_dp_logical_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {

    uint32_t sf = 0u;
    uint32_t opc = 0u;
    uint32_t n_bit = 0u;
    uint32_t rd = 0u;
    uint32_t rn = 0u;
    uint32_t rm = 0u;
    uint32_t shift_type = LOGICAL_SHIFT_LSL << LOGICAL_SHIFT_TYPE_SHIFT;
    uint32_t shift_amount = 0u;

    size_t rd_index = 0;
    size_t rn_index = 1u;
    size_t rm_index = 2u;
    size_t shift_index = 3u;
    if (strcmp(mnemonic,"tst") == 0) {
        if (operand_count != 2u && operand_count != 3u) {
            fprintf(stderr, "ERROR: Invalid operand count for %s\n", mnemonic);
            exit(1);
        }
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
    //TODO: MAKE THIS A DISPATCH TABLE
    //WHAT IS THIS FIRE TRUCKING SHOOT
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
    } else if (strcmp(mnemonic, "ands") == 0 || strcmp(mnemonic, "bics") == 0 || strcmp(mnemonic,"tst") == 0) {
        opc = OPC_ANDS;
    }

    opc = opc << DP_OPC_SHIFT;
    

    if (strcmp(mnemonic, "bic") == 0 || strcmp(mnemonic,"bics") == 0 || strcmp(mnemonic, "orn") == 0 || strcmp(mnemonic,"eon") == 0 || strcmp(mnemonic, "mvn") == 0) {
        n_bit = 1u << LOGICAL_NBIT_SHIFT;
    }

    const char *rd_text = operands[rd_index];
    const char *rn_text = operands[rn_index];
    const char *rm_text = operands[rm_index];

    rd = parse_reg(rd_text, &sf);
    if (strcmp(mnemonic, "tst") == 0) {
        rd = ZERO_REGISTER_NUMBER;
    }
    sf = sf << SF_SHIFT;
    uint32_t dummy_sf;
    rn = parse_reg(rn_text, &dummy_sf);
    rn = rn << LOGICAL_RN_SHIFT;
    rm = parse_reg(rm_text, &dummy_sf);
    rm = rm << LOGICAL_RM_SHIFT;

    if (operand_count > shift_index) {
        //yes shift
        const char *shift_text = operands[shift_index];
        if (strncmp(shift_text,"lsl",3) == 0) {
            shift_type = LOGICAL_SHIFT_LSL;
        } else if (strncmp(shift_text,"lsr",3) == 0) {
            shift_type = LOGICAL_SHIFT_LSR;
        } else if (strncmp(shift_text,"asr",3) == 0) {
            shift_type = LOGICAL_SHIFT_ASR;
        } else if (strncmp(shift_text, "ror", 3) == 0) {
            shift_type = LOGICAL_SHIFT_ROR;
        } else {
            fprintf(stderr, "ERROR: Invalid shift type %s\n", shift_text);
            exit(1);
        }

        shift_type = shift_type << LOGICAL_SHIFT_TYPE_SHIFT;

        if (operands[shift_index + 1][0] != '#') {
            fprintf(stderr, "ERROR: Logical shift amount must begin with #\n");
            exit(1);
        }
        shift_amount = read_number_or_label(operands[shift_index + 1]);
        if (sf == 0) {
            if (shift_amount > MAX_32BIT_SHIFT_AMOUNT) {
                fprintf(stderr, "ERROR: 32-bit logical shift amount is too large %d\n", shift_amount);
                exit(1);
            }
        } else if (shift_amount > MAX_64BIT_SHIFT_AMOUNT) {
            fprintf(stderr, "ERROR: 64-bit logical shift amount is too large %d\n", shift_amount);
            exit(1);
        }

        shift_amount = shift_amount << LOGICAL_SHIFT_AMOUNT_SHIFT;
    }
    return sf | opc | DP_REG_FIXED_BITS | shift_type | n_bit | rm | shift_amount | rn | rd;
}