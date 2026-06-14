//
// Created by Rayan Abdallah on 14/06/2026.
//

#include "ass_single_data_transfer.h"

uint32_t ass_single_data_transfer(char* mnemonic, char *operands[],
                    size_t operand_count, uint32_t pc)
{
    if (operand_count < 2 || operand_count > 3) {
        fprintf(stderr, "wrong number of operands");
        exit(1);
    }
    uint32_t res = 0;
    uint8_t reg_t = atoi(&operands[0][1]);
    bool in_64_bits = false;
    if (operands[0][0] == 'X')
    {
        res |= 1 << SF_BIT;
        in_64_bits = true;
    }
    if (operands[1][0] != '[') // LOAD LITERAL
    {
        res |= LOAD_LITERAL_BITS << LOAD_LITERAL_BITS_POS;
    }
    if (strcmp(mnemonic, "ldr"))
    {
        res |= 1 << MOST_SIG_BIT;
        res |= 1 << L_BIT;
    }
    else if (strcmp(mnemonic, "str"))
    {
        uint32_t res = 1 << MOST_SIG_BIT;
    }
    else
    {
        fprintf(stderr, "wrong mnemonic");
        exit(1);
        return 0;
    }
}