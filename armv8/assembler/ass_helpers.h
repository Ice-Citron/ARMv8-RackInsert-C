#ifndef ASSHELPERS
#define ASSHELPERS
//for helpers which run independently of the operation
#include "assemble.h"
#include "symbol_table.h"

#define SHIFT_LSL 0u
#define SHIFT_LSR 1u
#define SHIFT_ASR 2u
#define SHIFT_ROR 3u
#define LENGTH_OF_SHIFT_COMMAND 3

typedef struct {
    char* instrname;
    int index;
} idx_operand_no;

typedef struct {
    char* instrname;
    int lesser_ops;
    int greater_ops;
} opcount_checker;

void print_error_and_exit(const char *complaint);
void check_opcount(opcount_checker *opcount_table, int opct_length, char* target, int opcount);
void check_shift_amt(uint32_t shift_amount, uint32_t sf);
uint32_t read_number_or_label(char *string);
uint32_t parse_reg(const char *text, uint32_t *sf);


#endif