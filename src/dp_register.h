#include "dp_shared.h"

#define SF_DPREG_START 31
#define SF_DPREG_END 31
#define OPC_DPREG_START 30
#define OPC_DPREG_END 29
#define M_DPREG_START 28
#define M_DPREG_END 28
#define RD_DPREG_START 4
#define RD_DPREG_END 0
#define RN_DPREG_START 9
#define RN_DPREG_END 5
#define RM_DPREG_START 20
#define RM_DPREG_END 16

#define M_DPREG_MULTIPLY 1
#define RA_DPREG_MULT_START 14
#define RA_DPREG_MULT_END 10
#define X_DPREG_MULT_START 15
#define X_DPREG_MULT_END 15
#define X_DPREG_MADD 0

#define OPR0_DPREG_START 24
#define OPR0_DPREG_END 24
#define OPR0_DPREG_ARITH 1
#define SHIFT_DPREG_START 23
#define SHIFT_DPREG_END 22
#define SHIFT_DIST_DPREG_START 15
#define SHIFT_DIST_DPREG_END 10
#define N_DPREG_START 21
#define N_DPREG_END 21

#define SHIFT_TYPE_LSL 0
#define SHIFT_TYPE_LSR 1
#define SHIFT_TYPE_ASR 2

#define OPC_DPREG_AND 0
#define OPC_DPREG_ORR 1
#define OPC_DPREG_EOR 2

void dpreg(uint32_t instr);