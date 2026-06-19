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

#define DPIMM_OPC_TABLE_LENGTH 3
#define LENGTH_OF_SHIFT_COMMAND 3

#define SHIFT_LSL 0u
#define SHIFT_LSR 1u
#define SHIFT_ASR 2u
#define SHIFT_ROR 3u

extern opcount_checker immreg_opcounts[];
void check_opcode (char* mnemonic, uint32_t *opc);
uint32_t decide_imm_or_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc);

#endif