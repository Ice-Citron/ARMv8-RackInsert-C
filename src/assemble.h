#ifndef ASSEMBLE_H
#define ASSEMBLE_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//ONLY things which exist independent of file-input
//AND things relating to main function

#define OUTPUT_BYTES_PER_LINE 4
#define BITS_IN_A_BYTE 8

#define BYTES_IN_32_BITS 4
#define BYTES_IN_64_BITS 8
#define LOG2_BYTES_IN_32_BITS 2
#define LOG2_BYTES_IN_64_BITS 3

#define MAIN_NUMBEROF_ARGUMENTS 3

#define REG_X_SF 1
#define REG_W_SF 0
#define ZERO_REGISTER_NUMBER 31

#define SF_SHIFT 31u
#define DP_OPC_SHIFT 29u
#define INSTR_COUNT 38

#define HEX_TYPE_SIGNATURE "0x"
#define LEN_HEX_TYPE_SIGNATURE 2
#define HEX_BASE 16
#define DECI_BASE 10

#define MAX_OPERANDS 5
#define HALT_ADDRESS 0x8a000000

#define MAX_32BIT_SHIFT_AMOUNT 31u
#define MAX_64BIT_SHIFT_AMOUNT 63u

#endif