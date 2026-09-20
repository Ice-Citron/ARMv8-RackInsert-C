#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_SYMBOL_LEN 64
#define MAX_SYMBOL_COUNT (1 << 21)

#define INITIAL_SYMTABLE_CAP 5
#define SYMTABLE_RESIZE_FACTOR 2
#define ADDRESS_FAIL -1

void init_symbol_table(void);

bool find_address_from_sym_table(const char *name, uint32_t *address);

void add_to_symbol_table(const char *name, const uint32_t address);

#endif