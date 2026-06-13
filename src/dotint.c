#include "dotint.h"
//pre: we get a single number, either in binary or hexadecimal 
//pre: assumed that the line is known to be .int
uint32_t int_directive (char* mnemonic, char *operands[], size_t operand_count) {
    if (strcmp(*mnemonic, ".int") != 0 || operand_count != 1) {
        fprintf(stderr, "Invalid instruction parameters");
        exit(1);
    }
    uint8_t outputbytes[OUTPUT_BYTES_PER_LINE];
    uint32_t inputint;
    char* endptr;
    if (strncmp(operands[0], HEX_TYPE_SIGNATURE, LEN_HEX_TYPE_SIGNATURE)) {
        //shows that it is hexadecimal
        inputint = strtol(operands[0], endptr, HEX_BASE);
    } else {
        //decimal support
        inputint = strtol(operands[0] + LEN_HEX_TYPE_SIGNATURE, endptr, HEX_BASE);
    }
    return inputint;
}

//post: the value will be returned as a uint32_t 
//standardise over all dispatches