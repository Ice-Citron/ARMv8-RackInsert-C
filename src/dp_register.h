#ifndef DP_REGISTER_H
#define DP_REGISTER_H

#include "dp_shared.h"

#define SF_REG_HI  31
#define SF_REG_LO  31
#define OPC_REG_HI 30
#define OPC_REG_LO 29
#define M_REG_HI   28
#define M_REG_LO   28
#define RD_REG_HI  4
#define RD_REG_LO  0
#define RN_REG_HI  9
#define RN_REG_LO  5
#define RM_REG_HI  20
#define RM_REG_LO  16

#define M_REG_MULTIPLY 1
#define RA_REG_MULT_HI 14
#define RA_REG_MULT_LO 10
#define X_REG_MULT_HI  15
#define X_REG_MULT_LO  15
#define X_REG_MADD     0

#define OPR0_REG_HI       24
#define OPR0_REG_LO       24
#define OPR0_REG_ARITH    1
#define SHIFT_REG_HI      23
#define SHIFT_REG_LO      22
#define SHIFT_DIST_REG_HI 15
#define SHIFT_DIST_REG_LO 10
#define N_REG_HI          21
#define N_REG_LO          21

#define SHIFT_TYPE_LSL 0
#define SHIFT_TYPE_LSR 1
#define SHIFT_TYPE_ASR 2
#define SHIFT_TYPE_ROR 3

#define OPC_REG_AND        0
#define OPC_REG_ORR        1
#define OPC_REG_EOR        2
#define OPC_REG_ANDs_BRICS 3

#endif
