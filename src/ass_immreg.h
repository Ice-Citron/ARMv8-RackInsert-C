#ifndef IMMREG
#define IMMREG

#include "assemble_dp_imm.h"
#include "assemble_dp_reg.h"

#define IMMREG_INSTR_COUNT 8
#define IMMEDIATE_CHARACTER '#'

#define OPC_ADD 0u
#define OPC_ADDS 1u
#define OPC_SUB 2u
#define OPC_SUBS 3u

#define CMPCMN_OPCOUNT_NOSHIFT 2
#define CMPCMN_OPCOUNT_SHIFT 4
#define NEGNEGS_OPCOUNT_NOSHIFT 2
#define NEGNEGS_OPCOUNT_SHIFT 4
#define OTHER_DPIMM_OPCOUNT_NOSHIFT 3
#define OTHER_DPIMM_OPCOUNT_SHIFT 5

typedef struct {
    char* instrname;
    int index;
} idx_operand_no;

uint32_t decide_imm_or_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

#endif