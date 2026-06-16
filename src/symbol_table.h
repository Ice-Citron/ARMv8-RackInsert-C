#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOL_LEN 64
#define MAX_SYMBOL_COUNT (1 << 21)

void init_symbol_table();

uint32_t find_address_from_sym_table(const char *name);

void add_to_symbol_table(const char *name, const uint32_t address);

#endif