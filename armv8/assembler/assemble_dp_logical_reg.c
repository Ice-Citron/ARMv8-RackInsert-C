#include "assemble_dp_logical_reg.h"
#include "assemble_file.h"

static opcount_checker dplogic_opcounts[] = {
	{"tst", TST_OPCOUNT_NOSHIFT, TST_OPCOUNT_SHIFT},
	{"movmvn", MOVMVN_OPCOUNT_NOSHIFT, MOVMVN_OPCOUNT_SHIFT},
	{"other", OTHER_LOGIC_OPCOUNT_NOSHIFT, OTHER_LOGIC_OPCOUNT_SHIFT},
};

uint32_t assemble_dp_logical_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {
    uint32_t sf = 0u;
    uint32_t opc = 0u;
    uint32_t n_bit = 0u;
    uint32_t rd = 0u;
    uint32_t rn = 0u;
    uint32_t rm = 0u;
    uint32_t shift_type = SHIFT_LSL << LOGICAL_SHIFT_TYPE_SHIFT;
    uint32_t shift_amount = 0u;

    size_t rd_index = 0;
    size_t rn_index = 1u;
    size_t rm_index = 2u;
    size_t shift_index = 3u;

    if (strcmp(mnemonic,"tst") == 0) {
        check_opcount(dplogic_opcounts, DPLOGIC_OPC_TABLE_LENGTH, "tst", operand_count);
        rn_index = 0u;
        rm_index = 1u;
        shift_index = 2u;
    } else if (strcmp(mnemonic,"mov") == 0 || strcmp(mnemonic, "mvn") == 0) {
        check_opcount(dplogic_opcounts, DPLOGIC_OPC_TABLE_LENGTH, "movmvn", operand_count);
        rn = ZERO_REGISTER_NUMBER;
        rd_index = 0u;
        rm_index = 1u;
        shift_index = 2u;
    } else {
        check_opcount(dplogic_opcounts, DPLOGIC_OPC_TABLE_LENGTH, "other", operand_count);
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
    } else if (strcmp(mnemonic, "ands") == 0 || strcmp(mnemonic, "bics") == 0 ||
        strcmp(mnemonic,"tst") == 0) {
        opc = OPC_ANDS;
    }

    opc = opc << DP_OPC_SHIFT;
    

    if (strcmp(mnemonic, "bic") == 0 || strcmp(mnemonic,"bics") == 0 || 
        strcmp(mnemonic, "orn") == 0 || strcmp(mnemonic,"eon") == 0 || 
        strcmp(mnemonic, "mvn") == 0) {
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
    if (strcmp(mnemonic, "mov") == 0 || strcmp(mnemonic, "mvn") == 0) {
        rn = ZERO_REGISTER_NUMBER;
    }
    rn = rn << LOGICAL_RN_SHIFT;
    rm = parse_reg(rm_text, &dummy_sf);
    rm = rm << LOGICAL_RM_SHIFT;

    if (operand_count > shift_index) {
        //yes shift
        const char *shift_text = operands[shift_index];
        if (strncmp(shift_text, "lsl", LENGTH_OF_SHIFT_COMMAND) == 0) {
            shift_type = SHIFT_LSL;
        } else if (strncmp(shift_text, "lsr", LENGTH_OF_SHIFT_COMMAND) == 0) {
            shift_type = SHIFT_LSR;
        } else if (strncmp(shift_text, "asr", LENGTH_OF_SHIFT_COMMAND) == 0) {
            shift_type = SHIFT_ASR;
        } else if (strncmp(shift_text, "ror", LENGTH_OF_SHIFT_COMMAND) == 0) {
            shift_type = SHIFT_ROR;
        } else {
            print_error_and_exit("INVALID SHIFT TYPE");
        }

        shift_type = shift_type << LOGICAL_SHIFT_TYPE_SHIFT;
        if (operands[shift_index + 1][0] != '#') {
            print_error_and_exit("LOGICAL SHIFT MUST BEGIN WITH #");
        }
        shift_amount = read_number_or_label(operands[shift_index + 1]);
        check_shift_amt(shift_amount, sf);
        shift_amount = shift_amount << LOGICAL_SHIFT_AMOUNT_SHIFT;
    }
    return sf | opc | DP_REG_FIXED_BITS | shift_type | n_bit | rm | shift_amount | rn | rd;
}