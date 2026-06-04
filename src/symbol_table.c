#include "symbol_table.h"

typedef struct {
    uint32_t address;
    char name[MAX_SYMBOL_LEN];
} symbol_pair;

symbol_pair *symbol_table;
int symbol_table_size = 0;
int symbol_table_capacity = 0;

void init_symbol_table()
{
    symbol_table = (symbol_pair *) malloc(sizeof(symbol_pair) * 5);
    symbol_table_capacity = 5;
}

void resize_symbol_table()
{
    symbol_table = (symbol_pair *) realloc(symbol_table,
        sizeof(symbol_pair) * 2 * symbol_table_size);
    symbol_table_capacity = 2 * symbol_table_size;
}

uint32_t find_address_from_sym_table(const char *name)
{
    for (int i = 0; i < symbol_table_size; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
        {
            return symbol_table[i].address;
        }
    }
    return -1;
}

void add_to_symbol_table(const char *name, const uint32_t address)
{
    if (symbol_table_size == symbol_table_capacity)
    {
        resize_symbol_table();
    }
    strcpy(symbol_table[symbol_table_size].name, name);
    symbol_table[symbol_table_size].address = address;
    symbol_table_size++;
}