#include "symbol_table.h"

typedef struct {
    uint32_t address;
    char name[MAX_SYMBOL_LEN];
} symbol_pair;

static symbol_pair *symbol_table;
static int symbol_table_size = 0;
static int symbol_table_capacity = 0;

void init_symbol_table(void)
{
    symbol_table = (symbol_pair *) malloc(sizeof(symbol_pair) * INITIAL_SYMTABLE_CAP);
    symbol_table_capacity = INITIAL_SYMTABLE_CAP;
}

static void resize_symbol_table(void)
{
    symbol_table = (symbol_pair *) realloc(symbol_table,
        sizeof(symbol_pair) * SYMTABLE_RESIZE_FACTOR * symbol_table_size);
    symbol_table_capacity = SYMTABLE_RESIZE_FACTOR * symbol_table_size;
}

//changed to boolean to show success
//the address will be returned as a pointer
//because ADDRESS_FAIL was -1 which will be casted to 2^31-1
//creating a potential bug when the label associates with 0xFFFFFFFF in symtable
bool find_address_from_sym_table(const char *name, uint32_t *address)
{
    for (int i = 0; i < symbol_table_size; i++)
    {
        if (strcmp(symbol_table[i].name, name) == 0)
        {
            *address = symbol_table[i].address;
            return true;
        }
    }
    return false;
}

void add_to_symbol_table(const char *name, const uint32_t address)
{
    if (symbol_table_size == symbol_table_capacity)
    {
        resize_symbol_table();
    }
    strncpy(symbol_table[symbol_table_size].name, name, MAX_SYMBOL_LEN - 1);
    symbol_table[symbol_table_size].address = address;
    symbol_table_size++;
}