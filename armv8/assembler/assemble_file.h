#ifndef ASSEMBLE_FILE_H
#define ASSEMBLE_FILE_H

#include "ass_helpers.h"
#include "ass_immreg.h"

#define DP_FIXED_BIT (1u << 28u)
#define DP_REG_FIXED_BITS (10u << 24u)

#define CHARS_IN_FULL_LINE_BUFFER 512
#define STOP_INSTR_OPCOUNT 3

#define LABEL_LAST_CHAR ':'
#define TERMINATORS " \t\n\r"
#define TERMINATORS_AND_COMMA ", \t\n\r"

typedef struct {
    const char* instrname;
    uint32_t (*handler)(char *, char* [], size_t, uint32_t);
} instr_dispatch;

uint32_t read_number_or_label (char* string);
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

#endif