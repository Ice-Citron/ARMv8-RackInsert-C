#include "assemble_dp_imm.h"
#include "assemble_file.h"

uint32_t assemble_dp_imm(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {
    uint32_t sf = 0u;
    uint32_t opc = 0u;
    uint32_t rd = 0u;
    uint32_t rn = 0u;
    uint32_t imm12 = 0u;
    uint32_t sh = 0u;
    size_t rd_index = 0;
    size_t rn_index = 1;
    size_t imm_index = 2;
    bool use_rd = true;
    bool use_rn = true;

    if (strcmp(mnemonic, "cmp") == 0 || strcmp(mnemonic, "cmn") == 0) {
        check_opcount(immreg_opcounts, DPIMM_OPC_TABLE_LENGTH, "cmpcmn", operand_count);
        use_rd = false;
        rn_index = 0u;
        imm_index = 1u;
    } else if (strcmp(mnemonic, "neg") == 0 || strcmp(mnemonic, "negs") == 0) {
        check_opcount(immreg_opcounts, DPIMM_OPC_TABLE_LENGTH, "negnegs", operand_count);
        rn = ZERO_REGISTER_NUMBER;
        use_rn = false;
        rd_index = 0u;
        imm_index = 1u;
    } else {
        check_opcount(immreg_opcounts, DPIMM_OPC_TABLE_LENGTH, "other", operand_count);
    }
    
    check_opcode(mnemonic, &opc);

    rd = parse_reg(operands[rd_index], &sf);
    //gets sf
    if (!use_rd) {
        rd = ZERO_REGISTER_NUMBER;
    }
    sf = sf << SF_SHIFT;
    uint32_t dummy_sf;
    rn = use_rn ? parse_reg(operands[rn_index], &dummy_sf) : ZERO_REGISTER_NUMBER;
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

    if (operand_count == imm_index + DP_IMM_DISTANCE_IMM_SHIFTAMT) {
        const char *shift_name = operands[imm_index + 1u];
        if (strcmp(shift_name,"lsl") != 0) {
            fprintf(stderr, "ERROR: Arithmetic immediate instruction only supports lsl shift\n");
            exit(1);
        }

        uint32_t parsed_shift = read_number_or_label(operands[imm_index + DP_IMM_DISTANCE_IMM_SHIFTTYPE]);
        if (parsed_shift != 0 && parsed_shift != DP_IMM_LSL_AMOUNT) {
            fprintf(stderr, "ERROR: Arithmetic immediate shift must be lsl #12\n");
            exit(1);
        }
        //shifting by zero is not a shift
        if (parsed_shift != 0) {
            sh = 1u << DP_IMM_SH_SHIFT;
        }
    }
    return sf | opc | DP_FIXED_BIT | DP_IMM_ARITHMETIC_OPI | sh | imm12 | rn | rd ;
}