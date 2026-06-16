#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define OUTPUT_BYTES_PER_LINE 4
#define BITS_IN_A_BYTE 8

#define MAIN_NUMBEROF_ARGUMENTS 3

typedef struct {
    const char* instrname;
    uint32_t (*handler)(char *, char* [], size_t, uint32_t);
} instr_dispatch;

#endif