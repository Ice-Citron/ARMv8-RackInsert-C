#ifndef ASSEMBLE_FILE_H
#define ASSEMBLE_FILE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

#define ZERO_REGISTER_NUMBER 31u
#define DP_FIXED_BIT (1u << 28u)
#define DP_REG_FIXED_BITS (10u << 24u)

#define SF_SHIFT 31u
#define DP_OPC_SHIFT 29u

#define INSTR_COUNT 37
#define IMMREG_INSTR_COUNT 8
#define IMMEDIATE_CHARACTER '#'

#define HEX_TYPE_SIGNATURE "0x"
#define LEN_HEX_TYPE_SIGNATURE 2
#define HEX_BASE 16
#define DECI_BASE 10

uint32_t read_number_or_label (char* string) {
    uint32_t inputint;
    char* endptr;
    uint32_t maybe_symbol = find_address_from_sym_table(string);
    //check whether label
    if (maybe_symbol != ADDRESS_FAIL) {
        return maybe_symbol;
    } else if (strncmp(string, HEX_TYPE_SIGNATURE, LEN_HEX_TYPE_SIGNATURE)) {
        //check whether hexadecimal
        inputint = strtol(string + LEN_HEX_TYPE_SIGNATURE, &endptr, HEX_BASE);
        //pointer arithmetic above
    } else {
        //decimal
        inputint = strtol(string, &endptr, DECI_BASE);
    }
    return inputint;
}

typedef struct {
    char* instrname;
    int index;
} idx_operand_no;

typedef struct {
    const char* instrname;
    uint32_t (*handler)(char *, char* [], size_t, uint32_t);
} instr_dispatch;

uint32_t assemble_instructions(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_dp_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_dp_logical_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_multiply(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_dp_imm(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t assemble_wide_move(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

extern uint32_t ass_branch(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t ass_single_data_transfer(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);
extern uint32_t int_directive (char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

bool assemble_file(char *infile, char *outfile);

#endif // ASSEMBLE_FILE_H
