#include <assemble_dp.h>

// Return value of 0 indicates success, 1 indicates failure
uint32_t assemble_dp(string mnemonic, char *operands[], size_t operand_count) {
    if (strcmp(mnemonic,"movn") == 0 || strcmp(mnemonic,"movz") == 0 || strcmp(mnemonic,"movk") == 0) {
        return assemble_wide_move(mnemonic, operands, operand_count);
    } 
    else if (strcmp(mnemonic , "madd") == 0 || strcmp(mnemonic , "msub") == 0 || strcmp(mnemonic ,"mul") == 0 || strcmp(mnemonic , "mneg") == 0) {
        return assemble_multiply(mnemonic, operands, operand_count);
    }
    else if (strcmp(mnemonic , "add") == 0 || strcmp(mnemonic ,"adds") == 0 || strcmp(mnemonic , "sub") == 0
     || strcmp(mnemonic , "subs") == 0 || strcmp(mnemonic , "cmp") == 0 || strcmp(mnemonic, "cmn") == 0 || strcmp(mnemonic , "neg") == 0 || strcmp(mnemonic , "negs") == 0) {
        
        size_t operand_2_idx = 2;

        if (strcmp(mnemonic, "cmp") == 0 || strcmp(mnemonic, "cmn") == 0 || strcmp(mnemonic, "neg") == 0 || strcmp(mnemonic, "negs") == 0) {
            operand_2_idx = 1;
        }

        if (operands[operand_2_idx] [0] == '#') {
            return assemble_dp_imm(mnemonic, operands, operand_count);
        }
        else {
            return assemble_dp_reg(mnemonic, operands, operand_count);
        }
    }
    else if (strcmp(mnemonic , "and") == 0 || strcmp(mnemonic , "bic") == 0 || strcmp(mnemonic , "orr") == 0 || strcmp(mnemonic , "orn") == 0 || strcmp(mnemonic , "eor") == 0
        || strcmp(mnemonic ,"eon") == 0 || strcmp(mnemonic ,"ands") == 0 || strcmp(mnemonic , "bics") == 0 || strcmp(mnemonic , "tst") == 0 || strcmp(mnemonic , "mov") == 0 || strcmp(mnemonic , "mvn") == 0) {
            return assemble_logical_register(mnemomic, operands, operand_count);
        }

    }

static uint32_t assemble_wide_move(string mnemonic,char *operands, size_t operand_count) {
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

    char* end = NULL;
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
            frprintf(stderr, "Wide move only allows left shift");
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

    switch (mnemonic) {
            case "movn":
                opc = OPC_MOVN << DP_OPC_SHIFT;
                break;
            case  "movk":
                opc = OPC_MOVK << DP_OPC_SHIFT;
                break;
            case "movz":
                opc = OPC_MOVZ << DP_OPC_SHIFT;
                break;
            default:
                fprintf(stderr, "ERROR: Unknown wide move instruction %s",mnemonic)
        }

    uint32_t hw = (shift/WIDE_MOVE_SHIFT_UNIT ) << WIDE_MOVE_HW_SHIFT;
    imm16 = imm16 << WIDE_MOVE_IMM16_SHIFT

    *word_out = sf | opc | FIXED_BIT | WIDE_MOVE_OPI | hw | imm16 | rd;

}

static uint32_t assemble_multiply(mnemonic, char *operands[], size_t operand_count,) {

    uint32_t sf;
    uint32_t rm;
    uint32_t x;
    uint32_t ra;
    uint32_t rn;
    uint32_t rd;

    

}