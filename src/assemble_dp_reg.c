#include "assemble_dp_reg.h"

uint32_t assemble_dp_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {

    uint32_t sf = 0u;
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
    bool use_rd = true;
    bool use_rn = true;

    if (strcmp(mnemonic, "cmp") == 0 || strcmp(mnemonic, "cmn") == 0) {
        check_opcount(immreg_opcounts, DPIMM_OPC_TABLE_LENGTH, "cmpcmn", operand_count);
        use_rd = false;
        rn_index = 0u;
        rm_index = 1u;
        shift_index = 2u;
    } else if (strcmp(mnemonic, "neg") == 0 || strcmp(mnemonic, "negs") == 0) {
        check_opcount(immreg_opcounts, DPIMM_OPC_TABLE_LENGTH, "negnegs", operand_count);
        rn = ZERO_REGISTER_NUMBER;
        use_rn = false;
        rd_index = 0u;
        rm_index = 1u;
        shift_index = 2u;
    } else {
        check_opcount(immreg_opcounts, DPIMM_OPC_TABLE_LENGTH, "other", operand_count);
        shift_index = 3u;
        rd_index = 0u;
        rn_index = 1u;
        rm_index = 2u;
    }

    check_opcode(mnemonic, &opc);

    //should factor this out into function
    rd = parse_reg(operands[rd_index], &sf);
    //gets sf
    if (!use_rd) {
        rd = ZERO_REGISTER_NUMBER;
    }
    sf = sf << SF_SHIFT;
    uint32_t dummy_sf;
    rn = use_rn ? parse_reg(operands[rn_index], &dummy_sf) : ZERO_REGISTER_NUMBER;
    rn = rn << DP_REG_RN_SHIFT;
    rm = parse_reg(operands[rm_index], &dummy_sf);
    rm = rm << DP_REG_RM_SHIFT;

    if (operand_count > shift_index) {
        const char *shift_text = operands[shift_index];
        if (strncmp(shift_text, "lsl", LENGTH_OF_SHIFT_COMMAND) == 0) {
            shift_type = SHIFT_LSL;
        } else if (strncmp(shift_text, "lsr", LENGTH_OF_SHIFT_COMMAND) == 0) {
            shift_type = SHIFT_LSR;
        } else if (strncmp(shift_text, "asr", LENGTH_OF_SHIFT_COMMAND) == 0) {
            shift_type = SHIFT_ASR;
        } else {
            print_error_and_exit("INVALID SHIFT TYPE");
        }
        shift_type = shift_type << SHIFT_TYPE_SHIFT;

        char *amount_text = operands[shift_index + 1];
        shift_amount = read_number_or_label(amount_text);
        //this should really be a helper
        check_shift_amt(shift_amount, sf);
        shift_amount = shift_amount << DP_REG_SHIFTAMOUNT_SHIFT;
    }
    return sf | opc | DP_REG_FIXED_BITS | DP_REG_ARITHMETIC_OPR | shift_type | rm | shift_amount | rn | rd ;
}