#include "dotint.h"
//pre: we get a single number, either in binary or hexadecimal 
//pre: assumed that the line is known to be .int
uint8_t *int_directive (char* inputstr) {
    uint8_t outputbytes[OUTPUT_BYTES_PER_LINE];
    uint32_t inputint;
    char* endptr;
    if (strncmp(inputstr, HEX_TYPE_SIGNATURE, LEN_HEX_TYPE_SIGNATURE)) {
        //shows that it is hexadecimal
        inputint = strtol(inputstr, endptr, HEX_BASE);
    } else {
        //decimal support
        inputint = strtol(inputstr + LEN_HEX_TYPE_SIGNATURE, endptr, HEX_BASE);
    }
    for (int i = 0; i < OUTPUT_BYTES_PER_LINE; i++) {
        outputbytes[i] = (uint8_t) inputint;
        inputint = inputint >> BITS_IN_A_BYTE;
    }
    return outputbytes;
}

//post: the value will be returned as an int pointer 
//so it can be fwrite into the binary file in assemble_file.c