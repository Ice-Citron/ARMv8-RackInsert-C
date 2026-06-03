#include "symbol_table.h"
#include <stdlib.h>

typedef struct {
    uint32_t address;
    char name[MAX_SYMBOL_LEN];
} symbol_pair;

symbol_pair *symbol_table;
int symbol_table_size = 0;

void init_symbol_table()
{
    symbol_table = (symbol_pair *) malloc(sizeof(symbol_pair) * 5);
}

void resize_symbol_table()
{
    symbol_table = (symbol_pair *) realloc(symbol_table,
        sizeof(symbol_pair) * 2 * symbol_table_size);
}

uint32_t find_address_from_sym_table(const char *name)
{
    for (int i = 0; i < symbol_table_size; i++)
    {
        if (symbol_table[i].name == name)
        {
            return symbol_table[i].address;
        }
    }
    return -1;
}