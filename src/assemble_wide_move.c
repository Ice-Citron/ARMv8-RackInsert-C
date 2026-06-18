#include "assemble_wide_move.h"
#include "assemble_file.h"

#define WIDE_MOVE_OPI (5u << 23u)
#define WIDE_MOVE_HW_SHIFT 21u
#define WIDE_MOVE_IMM16_SHIFT 5u
#define WIDE_MOVE_SHIFT_UNIT 16u
#define WIDE_MOVE_MAX_SHIFT 48u
#define WIDE_MOVE_IMM16_MASK 0xffffu

#define OPC_MOVN 0u
#define OPC_MOVZ 2u
#define OPC_MOVK 3u

uint32_t assemble_wide_move(char* mnemonic,char *operands[], size_t operand_count, uint32_t pc) {
    uint32_t rd;
    uint32_t sf;
    uint32_t imm16;
    uint32_t shift = 0;
    uint32_t opc;

    //parse rd from operands[0] and imm16 from operands[1]
    if (operand_count != 2u && operand_count != 4u) {
        fprintf(stderr, "Wrong operand count for %s", mnemonic);
        exit(1);
    }
    if (operands[0] == NULL || operands[1] == NULL) {
        fprintf(stderr, "Insufficient operands for wide move");
        exit(1);
    }
    rd = parse_reg(operands[0], &sf);
    sf = sf << SF_SHIFT;

    //sanitising imm16
    if (operands[1][0] != '#') {
        fprintf(stderr, "wide move immediate must start with #");
        exit(1);
    }
    imm16 = read_number_or_label(operands[1]);
    if ((imm16 & ~WIDE_MOVE_IMM16_MASK) != 0u) {
        fprintf(stderr, "Wide move immediate value is larger than 16 bits\n");
        exit(1);
    }

    //parse shift from operands[2]
    //4 for 4 tokens rd, imm, lsl, #sh
    if (operand_count == 4u) {
        if (strncmp(operands[2], "lsl", 3) != 0) {
            fprintf(stderr, "Wide move only allows left shift");
            exit(1); 
        }
        char *shift_text = operands[3]; 
        while (*shift_text == ' ') {
            shift_text++;
        }

        if (*shift_text != '#') {
            fprintf(stderr, "ERROR: Wide move shift amount must begin with #");
            exit(1);
        }
        shift = read_number_or_label(shift_text);
    }

    if (strcmp(mnemonic, "movn") == 0) {
        opc = OPC_MOVN << DP_OPC_SHIFT;
    } else if (strcmp(mnemonic, "movk") == 0) {
        opc = OPC_MOVK << DP_OPC_SHIFT;
    } else if (strcmp(mnemonic, "movz") == 0) {
        opc = OPC_MOVZ << DP_OPC_SHIFT;
    } else {
        fprintf(stderr, "ERROR: Unknown wide move instruction %s",mnemonic);
        exit(1);
    }

    uint32_t hw = (shift/WIDE_MOVE_SHIFT_UNIT ) << WIDE_MOVE_HW_SHIFT;
    imm16 = imm16 << WIDE_MOVE_IMM16_SHIFT;

    return sf | opc | DP_FIXED_BIT | WIDE_MOVE_OPI | hw | imm16 | rd;

}