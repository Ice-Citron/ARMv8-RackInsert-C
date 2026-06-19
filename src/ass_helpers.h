#ifndef ASSHELPERS
#define ASSHELPERS
//for helpers which run independently of the operation
#include "assemble.h"
#include "symbol_table.h"

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
uint32_t read_number_or_label(char *string);
uint32_t parse_reg(const char *text, uint32_t *sf);


#endif