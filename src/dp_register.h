#ifndef DP_REGISTER_H
#define DP_REGISTER_H

#include "dp_shared.h"

#define SF_DPREG_HI  31
#define SF_DPREG_LO  31
#define OPC_DPREG_HI 30
#define OPC_DPREG_LO 29
#define M_DPREG_HI   28
#define M_DPREG_LO   28
#define RD_DPREG_HI  4
#define RD_DPREG_LO  0
#define RN_DPREG_HI  9
#define RN_DPREG_LO  5
#define RM_DPREG_HI  20
#define RM_DPREG_LO  16

#define M_DPREG_MULTIPLY 1
#define RA_DPREG_MULT_HI 14
#define RA_DPREG_MULT_LO 10
#define X_DPREG_MULT_HI 15
#define X_DPREG_MULT_LO 15
#define X_DPREG_MADD 0

#define OPR0_DPREG_HI 24
#define OPR0_DPREG_LO 24
#define OPR0_DPREG_ARITH 1
#define SHIFT_DPREG_HI 23
#define SHIFT_DPREG_LO 22
#define SHIFT_DIST_DPREG_HI 15
#define SHIFT_DIST_DPREG_LO 10
#define N_DPREG_HI 21
#define N_DPREG_LO 21

#define SHIFT_TYPE_LSL 0
#define SHIFT_TYPE_LSR 1
#define SHIFT_TYPE_ASR 2
#define SHIFT_TYPE_ROR 3

#define OPC_DPREG_AND        0
#define OPC_DPREG_ORR        1
#define OPC_DPREG_EOR        2
#define OPC_DPREG_ANDs_BRICS 3

#endif
