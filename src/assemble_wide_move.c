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

    if (operands[0][0] != 'x' && operands[0][0] != 'w') {
        fprintf(stderr, "Invalid register\n");
        exit(1);
    }

    if (operand_count != 2u && operand_count != 3u) {
        fprintf(stderr, "Wrong operand count for %s", mnemonic);
        exit(1);
    }

    if (operands[0] == NULL || operands[1] == NULL) {
        fprintf(stderr, "Insufficient operands for wide move");
        exit(1);
    }
    
    sf = (operands[0][0] == 'x' ? 1u: 0u) << SF_SHIFT;

    if (strcmp(operands[0] + 1, "zr")) {
        rd = ZERO_REGISTER_NUMBER;
    }

    char* end = NULL;
    unsigned long reg = strtoul(operands[0] + 1, &end, 10);

    if (*end != '\0' || reg > ZERO_REGISTER_NUMBER) {
        rd = ZERO_REGISTER_NUMBER;
    }

    rd = (uint32_t) reg;
    unsigned long value = strtoul(operands[1] + 1, &end, 0);

    if (*end != '\0') {
        fprintf(stderr, "Invalid immediate\n");
        exit(1);
    }
    
    imm16 =  (uint32_t) value;

    if ((imm16 & ~WIDE_MOVE_IMM16_MASK) != 0u) {
        fprintf(stderr, "Wide move immediate value is larger than 16 bits\n");
        exit(1);
    }

    //parse shift from operands[3]

    if (operand_count == 3u) {
        if (strncmp(operands[3], "lsl", 3) != 0) {
            fprintf(stderr, "Wide move only allows left shift");
            exit(1); 
        }

        const char *shift_text = operands[3] + 3;
        while (*shift_text == ' ') {
            shift_text++;
        }

        if (*shift_text != '#') {
            fprintf(stderr, "ERROR: Wide move shift amount must begin with #");
            exit(1);
        }

        char *shift_end = NULL;
        shift = (uint32_t) strtoul(shift_text + 1, &shift_end, 10);

    }

    if (strcmp(mnemonic, "movn") == 0) {
        opc = OPC_MOVN << DP_OPC_SHIFT;
    } else if (strcmp(mnemonic, "movk") == 0) {
        opc = OPC_MOVK << DP_OPC_SHIFT;
    } else if (strcmp(mnemonic, "movz") == 0) {
        opc = OPC_MOVZ << DP_OPC_SHIFT;
    } else {
        fprintf(stderr, "ERROR: Unknown wide move instruction %s",mnemonic);
    }

    uint32_t hw = (shift/WIDE_MOVE_SHIFT_UNIT ) << WIDE_MOVE_HW_SHIFT;
    imm16 = imm16 << WIDE_MOVE_IMM16_SHIFT;

    return sf | opc | DP_FIXED_BIT | WIDE_MOVE_OPI | hw | imm16 | rd;

}