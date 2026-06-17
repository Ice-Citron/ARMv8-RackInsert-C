#include "assemble_dp_reg.h"
#include "assemble_file.h"

#define DP_REG_ARITHMETIC_OPR (8u << 21u)

#define DP_REG_RM_SHIFT 16u
#define DP_REG_SHIFTAMOUNT_SHIFT 10u
#define DP_REG_RN_SHIFT 5u

#define SHIFT_LSL 0u
#define SHIFT_LSR 1u
#define SHIFT_ASR 2u

#define SHIFT_TYPE_SHIFT 22u

#define MAX_32BIT_SHIFT_AMOUNT 31u
#define MAX_64BIT_SHIFT_AMOUNT 63u

#define OPC_ADD 0u
#define OPC_ADDS 1u
#define OPC_SUB 2u
#define OPC_SUBS 3u

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

        if (operand_count != 2u && operand_count != 4u) {
            fprintf(stderr, "ERROR: Wrong operand count for %s\n", mnemonic);
            exit(1);
        }
        use_rd = false;
        rn_index = 0u;
        rm_index = 1u;
        shift_index = 2u;
    } else if (strcmp(mnemonic, "neg") == 0 || strcmp(mnemonic, "negs") == 0) {
        if (operand_count != 2u && operand_count != 4u) {
            fprintf(stderr, "ERROR: Wrong operand count for %s\n", mnemonic);
            exit(1);
        }
        rn = ZERO_REGISTER_NUMBER;
        use_rn = false;
        rd_index = 0u;
        rm_index = 1u;
        shift_index = 2u;
    } else {
        if (operand_count != 3u && operand_count != 5u) {
            fprintf(stderr, "ERROR: Wrong operand count for %s\n", mnemonic);
            exit(1);
        }
        shift_index = 3u;
        rd_index = 0u;
        rn_index = 1u;
        rm_index = 2u;
    }

    if (strcmp(mnemonic,"add") == 0) {
        opc = OPC_ADD;
    } else if (strcmp(mnemonic, "adds") == 0 || strcmp(mnemonic, "cmn") == 0) {
        opc = OPC_ADDS;
    } else if (strcmp(mnemonic, "sub") == 0 || strcmp(mnemonic, "neg") == 0) {
        opc = OPC_SUB;
    } else {
        opc = OPC_SUBS;
    }
    opc = opc << DP_OPC_SHIFT;

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
        if (strncmp(shift_text, "lsl", 3) == 0) {
            shift_type = SHIFT_LSL;
        } else if (strncmp(shift_text, "lsr", 3) == 0) {
            shift_type = SHIFT_LSR;
        } else if (strncmp(shift_text, "asr", 3) == 0) {
            shift_type = SHIFT_ASR;
        } else {
            fprintf(stderr, "ERROR: invalid shift type %s\n", shift_text);
            exit(1);
        }
        shift_type = shift_type << SHIFT_TYPE_SHIFT;

        char *amount_text = operands[shift_index + 1];
        shift_amount = read_number_or_label(amount_text);
        //this should really be a helper
        if (sf == 0u) {
            if (shift_amount > MAX_32BIT_SHIFT_AMOUNT) {
                fprintf(stderr, "ERROR: Shift amount too large for 32 bit registers\n");
                exit(1);
            }
        } else if (shift_amount > MAX_64BIT_SHIFT_AMOUNT) {
            fprintf(stderr, "ERROR: Shift amount too large for 64 bit register\n");
            exit(1);
        }
        shift_amount = shift_amount << DP_REG_SHIFTAMOUNT_SHIFT;
    }
    return sf | opc | DP_REG_FIXED_BITS | DP_REG_ARITHMETIC_OPR | shift_type | rm | shift_amount | rn | rd ;
}