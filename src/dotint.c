#include "dotint.h"
//pre: we get a single number, either in binary or hexadecimal 
//pre: assumed that the line is known to be .int
uint32_t int_directive (char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {
    if (strcmp(mnemonic, ".int") != 0 || operand_count != 1) {
        fprintf(stderr, "Invalid instruction parameters");
        exit(1);
    }
    return read_number_or_label(operands[0]);
}
//post: save the operand into the position at the program counter