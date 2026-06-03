#ifndef DPIMMH
#define DPIMMH

#include "dp_shared.h"

#define SF_IMM_HI  31
#define SF_IMM_LO  31
#define OPC_IMM_HI 30
#define OPC_IMM_LO 29
#define OPI_IMM_HI 25
#define OPI_IMM_LO 23
#define RD_IMM_HI  4
#define RD_IMM_LO  0

#define OPI_IMM_ARITH   2
#define RN_IMM_ARITH_HI 9
#define RN_IMM_ARITH_LO 5
#define SH_IMM_ARITH_HI 22
#define SH_IMM_ARITH_LO 22
#define IMM12_IMM_HI    21
#define IMM12_IMM_LO    10
#define IMM_ARITH_SHIFT_AMOUNT 12

#define OPI_IMM_WIDE_MOVE 5
#define SH_IMM_WM_HI      22
#define SH_IMM_WM_LO      21
#define IMM16_IMM_WM_HI   20
#define IMM16_IMM_WM_LO   5

#define IMM_WIDE_MOVE_SHIFT_FACTOR 16
#define OPC_IMM_MOVN 0
#define OPC_IMM_MOVZ 2
#define OPC_IMM_MOVK 3

#endif
