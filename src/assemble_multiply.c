#include "assemble_wide_move.h"
#include "assemble_file.h"

#define DP_REG_FIXED_BITS (10u << 24u) 

#define MULTIPLY_M_BIT (1u<<28u)
#define MULTIPLY_OPR (8u << 21u)
#define MULTIPLY_RM_SHIFT 16u
#define MULTIPLY_X_SHIFT 15u
#define MULTIPLY_RA_SHIFT 10u
#define MULTIPLY_RN_SHIFT 5u
#define MAX_REGISTER_NUMBER 31u

uint32_t assemble_multiply(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {

    uint32_t sf = 0u << SF_SHIFT;
    uint32_t rm = 0u;
    uint32_t x = 0u << MULTIPLY_X_SHIFT;
    uint32_t ra = ZERO_REGISTER_NUMBER << MULTIPLY_RA_SHIFT;
    uint32_t rn = 0u;
    uint32_t rd = 0u;

    bool is_mul_or_mneg = (strcmp(mnemonic, "mul") == 0 || strcmp(mnemonic, "mneg") == 0);

    if ( is_mul_or_mneg && (operand_count != 3 && operand_count != 4) ) {
        fprintf(stderr, "ERROR: Wrong operand count for %s\n", mnemonic);
        exit(1);
    }

    if (operands[0] == NULL || operands[1] == NULL || operands[2] == NULL) {
        fprintf(stderr, "ERROR: Missing operand for %s\n", mnemonic);
        exit(1);
    }

    if (operands[0][0] == 'x') {
        sf = 1u << SF_SHIFT;
    } else if (operands[0][1] != 'w') {
        fprintf(stderr, "ERROR: Invalid format for destination register\n");
        exit(1);
    }

    char *end = NULL;

    rd = (uint32_t) strtoul(operands[0] + 1, &end, 10);

    if (*end != '\0' || rd > MAX_REGISTER_NUMBER) {
        fprintf(stderr, "ERROR: Invalid format for rd for %s\n",mnemonic);
        exit(1);
    }

    //rd does not need to be shifted

    end = NULL;

    rn = (uint32_t) strtoul(operands[1] + 1, &end,10);
    if (*end != '\0' || rn > MAX_REGISTER_NUMBER) {
        fprintf(stderr, "ERROR: Invalid format for rn for %s\n",mnemonic);
        exit(1);
    }
    rn = rn << MULTIPLY_RN_SHIFT;

    end = NULL;

    rm = (uint32_t) strtoul(operands[2] + 1, &end, 10);
    if (*end != '\0' || rm > MAX_REGISTER_NUMBER) {
        fprintf(stderr, "ERROR: Invalid format for rm for %s\n",mnemonic);
        exit(1);
    }
    rm = rm << MULTIPLY_RM_SHIFT;

    if (!is_mul_or_mneg) {
        end = NULL;
        ra = (uint32_t) strtoul(operands[3] + 1, &end, 10);
        if (*end != '\0' || ra > MAX_REGISTER_NUMBER) {
            fprintf(stderr, "ERROR: Invalid format for ra%s\n", mnemonic);
            exit(1);
        }
        ra = ra << MULTIPLY_RA_SHIFT;
    }

    if (strcmp(mnemonic, "msub") == 0 || strcmp(mnemonic, "mneg") == 0) {
        x = 1u << MULTIPLY_X_SHIFT;
    }

    return sf | DP_REG_FIXED_BITS | MULTIPLY_M_BIT | MULTIPLY_OPR | rm | x | ra | rn | rd ;
}