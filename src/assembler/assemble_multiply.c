#include "assemble_multiply.h"

static opcount_checker mult_opcounts[] = {
	{"mulmneg", MULMNEG_OPCOUNT, MULMNEG_OPCOUNT},
	{"other", OTHER_MUL_OPCOUNT, OTHER_MUL_OPCOUNT},
};

uint32_t assemble_multiply(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {
    uint32_t sf = 0u << SF_SHIFT;
    uint32_t rm = 0u;
    uint32_t x = 0u << MULTIPLY_X_SHIFT;
    uint32_t ra = ZERO_REGISTER_NUMBER << MULTIPLY_RA_SHIFT;
    uint32_t rn = 0u;
    uint32_t rd = 0u;

    bool is_mul_or_mneg = (strcmp(mnemonic, "mul") == 0 || strcmp(mnemonic, "mneg") == 0);
    if (is_mul_or_mneg) {
        check_opcount(mult_opcounts, LENGTH_MULT_OPCOUNTS, "mulmneg", operand_count);
    } else {
        check_opcount(mult_opcounts, LENGTH_MULT_OPCOUNTS, "other", operand_count);
    }

    if (operands[0] == NULL || operands[1] == NULL || operands[2] == NULL) {
        print_error_and_exit("MISSING OPERAND");
    }
    rd = parse_reg(operands[0], &sf);
    sf = sf << SF_SHIFT;
    uint32_t dummy_sf;
    //rd does not need to be shifted
    rn = parse_reg(operands[1], &dummy_sf);
    rn = rn << MULTIPLY_RN_SHIFT;
    rm = parse_reg(operands[2], &dummy_sf);
    rm = rm << MULTIPLY_RM_SHIFT;

    if (!is_mul_or_mneg) {
        ra = parse_reg(operands[3], &dummy_sf);
        ra = ra << MULTIPLY_RA_SHIFT;
    }
    if (strcmp(mnemonic, "msub") == 0 || strcmp(mnemonic, "mneg") == 0) {
        x = 1u << MULTIPLY_X_SHIFT;
    }
    return sf | DP_REG_FIXED_BITS | MULTIPLY_M_BIT | MULTIPLY_OPR | rm | x | ra | rn | rd ;
}