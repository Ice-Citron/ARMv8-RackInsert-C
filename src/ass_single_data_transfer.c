#include "ass_single_data_transfer.h"
#include "symbol_table.h"
#include "assemble.h"

uint32_t ass_single_data_transfer(char* mnemonic, char *operands[],
                    size_t operand_count, uint32_t pc)
{
    if (operand_count < 2 || operand_count > 3) {
        fprintf(stderr, "wrong number of operands");
        exit(1);
    }
    uint32_t res = 0;
    uint8_t reg_t = atoi(&operands[0][1]);
    int num_bytes = 4;
    if (operands[0][0] == 'X')
    {
        res |= 1 << SF_BIT;
        num_bytes = 8;
    }
    if (operands[1][0] != '[') // LOAD LITERAL
    {
        res |= LOAD_LITERAL_BITS << LOAD_LITERAL_BITS_POS;
        int target_addr = 0;
        if (operands[1][0] == '#') // literal
        {
            target_addr = atoi(&operands[1][1]);
        }
        else // label
        {
            target_addr = find_address_from_sym_table(operands[1]);
        }
        int64_t offset = offset = (int64_t)target_addr - (int64_t)pc;
        int64_t simm19 = (offset >> 2) & SIM_19_BIT_MASK;
        res |= simm19 << SIM_19_POS;
        res |= (reg_t & NUM_OF_REGISTERS);
        return res;
    }
    // single data transfer
    int xn_addr = atoi(&operands[1][1]);
    res |= 1 << MOST_SIG_BIT;
    if (strcmp(mnemonic, "ldr") == 0) // ldr
    {
        res |= 1 << L_BIT;
    }
    else if (strcmp(mnemonic, "str") != 0) // not ldr and not str
    {
        fprintf(stderr, "wrong mnemonic");
        exit(1);
        return 0;
    }
    char *addr_of_hash = strchr(mnemonic, '#');
    addr_of_hash++;
    int imm_val = atoi(addr_of_hash);

}