#include "assemble_dp_imm.h"
#include "assemble_file.h"

#define DP_IMM_SH_SHIFT 22u
#define DP_IMM_IMM12_SHIFT 10u
#define DP_IMM_RN_SHIFT 5u

#define DP_IMM_ARITHMETIC_OPI (2u << 23u)

#define DP_IMM_IMM12_MASK 0xfffu
#define DP_IMM_LSL_AMOUNT 12u

#define OPC_ADD 0u
#define OPC_ADDS 1u
#define OPC_SUB 2u
#define OPC_SUBS 3u

uint32_t assemble_dp_imm(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {
    uint32_t sf = 0u << SF_SHIFT;
    uint32_t opc = 0u;
    uint32_t rd = 0u;
    uint32_t rn = 0u;
    uint32_t imm12 = 0u;
    uint32_t sh = 0u << DP_IMM_SH_SHIFT;

    size_t rn_index = 1u;
    size_t imm_index = 2u;

    if (strcmp(mnemonic, "cmp") == 0 || strcmp(mnemonic, "cmn") == 0 ||
        strcmp(mnemonic, "neg") == 0 || strcmp(mnemonic, "negs") == 0) {
        if (operand_count != 2u && operand_count != 4u) {
            fprintf(stderr, "ERROR: wrong operand count for %s\n", mnemonic);
            exit(1);
        }
        rd = ZERO_REGISTER_NUMBER;
        rn_index = 0u;
        imm_index = 1u;
    } else {
        if (operand_count != 3u && operand_count != 5u) {
            fprintf(stderr, "ERROR: wrong operand count for %s\n", mnemonic);
            exit(1);
        }
    }

    if (strcmp(mnemonic, "add") == 0) {
        opc = OPC_ADD << DP_OPC_SHIFT;
    } else if (strcmp(mnemonic, "adds") == 0 || strcmp(mnemonic, "cmn") == 0) {
        opc = OPC_ADDS << DP_OPC_SHIFT;
    } else if (strcmp(mnemonic, "sub") == 0 || strcmp(mnemonic, "neg") == 0) {
        opc = OPC_SUB << DP_OPC_SHIFT;
    } else if (strcmp(mnemonic, "subs") == 0 || strcmp(mnemonic, "cmp") == 0) {
        opc = OPC_SUBS << DP_OPC_SHIFT;
    }

    rd = parse_reg(operands[0], &sf);
    sf = sf << SF_SHIFT;
    uint32_t dummy_sf;
    rn = parse_reg(operands[rn_index], &dummy_sf);
    rn = rn << DP_IMM_RN_SHIFT;

    //parsing the immediate
    if (operands[imm_index][0] != '#') {
        fprintf(stderr, "ERROR: immediate must begin with # %s\n", operands[imm_index]);
    }

    uint32_t parsed_imm = read_number_or_label(operands[imm_index]);
    if (parsed_imm > DP_IMM_IMM12_MASK) {
        fprintf(stderr, "ERROR: Invalid 12-bit imm%s\n", operands[imm_index]);
        exit(1);
    }

    imm12 = (uint32_t) parsed_imm << DP_IMM_IMM12_SHIFT;

    if (operand_count == imm_index + 3u) {
        const char *shift_name = operands[imm_index + 1u];
        if (strcmp(shift_name,"lsl") != 0) {
            fprintf(stderr, "ERROR: Arithmetic immediate instruction only supports lsl shift\n");
            exit(1);
        }

        uint32_t parsed_shift = read_number_or_label(operands[imm_index + 2u]);
        if (parsed_shift != 0 && parsed_shift != DP_IMM_LSL_AMOUNT) {
            fprintf(stderr, "ERROR: Arithmetic immediate shift must be lsl #12\n");
            exit(1);
        }

        sh = 1u << DP_IMM_SH_SHIFT;
    }

    rn = rn << DP_IMM_RN_SHIFT;

    return sf | opc | DP_FIXED_BIT | DP_IMM_ARITHMETIC_OPI | sh | imm12 | rn | rd ;
}