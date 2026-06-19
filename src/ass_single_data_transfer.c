#include "ass_single_data_transfer.h"
#include <stdio.h>
#include <stdlib.h>

//expectation: result = f86f68b4 	
// instruction = ldr	x20, [x5, x15]
uint32_t ass_single_data_transfer(char* mnemonic, char *operands[],
                                  size_t operand_count, uint32_t pc)
{
    //checking # operands
    if (operand_count < DATATRANSFER_MINOPS || operand_count > DATATRANSFER_MAXOPS) {
        print_error_and_exit("WRONG NUMBER OF OPERANDS");
    } 
    uint32_t sf = 0;
    //taking in the first varianble - target register
    uint32_t res = parse_reg(operands[0], &sf);
    int num_bytes = BYTES_IN_32_BITS;
    if (sf)
    {
        res |= 1 << SF_BIT;
        num_bytes = BYTES_IN_64_BITS;
    }
    //LOAD LITERAL CASE
    if (operands[1][0] != '[') // to not start with '['
    //necessarily a load literal
    {
        if (strcmp(mnemonic, "ldr") != 0) {
            print_error_and_exit("only ldr supports literal addressing");
        }
        //change to bitwise
        res |= LOAD_LITERAL_BITS << LOAD_LITERAL_BITS_POS;
        //i think this duplicates ass_branch.h helper
        uint32_t target_addr = read_number_or_label(operands[1]);
        int64_t offset = offset = (int64_t)target_addr - (int64_t)pc;
        int64_t simm19 = (offset >> LOG2_BYTES_IN_32_BITS) & SIM_19_BIT_MASK;
        res |= simm19 << SIM_19_POS;
        return res;
    }
    // single data transfer
    res |= 1 << MOST_SIG_BIT;
    res |= SINGLE_DATA_TRANSFER_BITS << SINGLE_DATA_TRANSFER_BITS_POS;
    int xn_addr = atoi(operands[1] + 2);
    //both for the (x or w) and the bracket
    res |= (xn_addr & XN_XM_BITMASK) << POS_OF_XN;
    //can't use the register function because there may be a trailing bracket
    //say for unsigned immediate offset
    if (strcmp(mnemonic, "ldr") == 0) // ldr
    {
        res |= 1 << L_BIT;
    }
    else if (strcmp(mnemonic, "str") != 0) // not ldr and not str
    {
        print_error_and_exit("MNEMONIC NEITHER LDR NOT STR");
        exit(1);
        return 0;
    }
    //WE SHOULD MAKE THIS A DISPATCH TABLE OF CONDITIONS? IDK
    if (LOAD_LITERAL_2OPS_COND) {
        res |= 1 << UNSIGNED_IMM_OFFSET_U_BIT;
        return res;
    }

    char *addr_of_hash = HASHTAG_ADDRESS_3OPS_LOAD_LITERAL;
    if (addr_of_hash == NULL) // register offset
    {
        int xm_addr = atoi(operands[2] + 1);
        //skip the hash
        res |= REGISTER_OFFSET_BITS << REGISTER_OFFSET_POS;
        res |= (xm_addr & XN_XM_BITMASK) << REGISTER_OFFSET_XM_POS;
        res |= 1 << REGISTER_OFFSET_BIT_POS;
        return res;
    }
    addr_of_hash ++;
    int imm_val = strtol(addr_of_hash, NULL, 0);

    if (UNSIGNED_IMM_OFFSET_COND) //unsigned imm offset
    {
        res |= 1 << UNSIGNED_IMM_OFFSET_U_BIT;
        if (num_bytes == BYTES_IN_32_BITS) // 32 bit
        {
            imm_val >>= LOG2_BYTES_IN_32_BITS; // divide by 4
        }
        else
        {
            imm_val >>= LOG2_BYTES_IN_64_BITS; // divide by 8
        }
        res |= (imm_val  << UNSIGNED_IMM_OFFSET_POS);
        return res;
    }
    // PRE AND POST INDEX
    res |= 1 << PRE_POST_INDEX_BIT_POS;
    res |= (imm_val & PRE_POST_INDEX_SIMM9_BITMASK) << PRE_POST_INDEX_SIMM9_POS;
    if (PRE_INDEX_COND) // PRE INDEX
    {
        res |= 1 << PRE_INDEX_BIT_POS;
    }
    //POST INDEX
    return res;
}
