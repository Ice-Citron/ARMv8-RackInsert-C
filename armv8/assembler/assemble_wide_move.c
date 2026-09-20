#include "assemble_wide_move.h"
#include "assemble_file.h"

static opcount_checker wm_opcounts[] = {
	{"wm", WM_NOSHIFT_OPCOUNT, WM_SHIFT_OPCOUNT},
};

uint32_t assemble_wide_move(char* mnemonic,char *operands[], size_t operand_count, uint32_t pc) {
    uint32_t rd;
    uint32_t sf;
    uint32_t imm16;
    uint32_t shift = 0;
    uint32_t opc;

    //parse rd from operands[0] and imm16 from operands[1]
    check_opcount(wm_opcounts, LENGTH_WM_OPCOUNTS, "wm", operand_count);
    if (operands[0] == NULL || operands[1] == NULL) {
        print_error_and_exit("INSUFFICIENT OPERANDS FOR WIDE MOVE");
    }
    rd = parse_reg(operands[0], &sf);
    sf = sf << SF_SHIFT;

    //sanitising imm16
    if (operands[1][0] != '#') {
        print_error_and_exit("WIDE MOVE IMMEDIATE MUST START WITH #");
    }
    imm16 = read_number_or_label(operands[1]);
    if ((imm16 & ~WIDE_MOVE_IMM16_MASK) != 0u) {
        print_error_and_exit("WIDE MOVE IMMEDIATE LARGER THAN 16 BITS");
    }

    //parse shift from operands[2]
    //4 for 4 tokens rd, imm, lsl, #sh
    if (operand_count == WM_SHIFT_OPCOUNT) {
        if (strncmp(operands[2], "lsl", LENGTH_OF_SHIFT_COMMAND) != 0) {
            print_error_and_exit("WIDE MOVE ONLY ALLOWS LEFT SHIFT");
        }
        char *shift_text = operands[WM_SHIFT_OPCOUNT - 1]; 
        while (*shift_text == ' ') {
            shift_text++;
        }

        if (*shift_text != '#') {
            print_error_and_exit("WIDE MOVE SHIFT AMOUNT MUST START WITH #");
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
        print_error_and_exit("UNKNOWN WIDE MOVE INSTRUCTION");
    }

    uint32_t hw = (shift/WIDE_MOVE_SHIFT_UNIT ) << WIDE_MOVE_HW_SHIFT;
    imm16 = imm16 << WIDE_MOVE_IMM16_SHIFT;
    return sf | opc | DP_FIXED_BIT | WIDE_MOVE_OPI | hw | imm16 | rd;
}