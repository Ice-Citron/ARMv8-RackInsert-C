#ifndef DPIMMH
#define DPIMMH

#include "dp_shared.h"

#define SF_DPIMM_HI 31
#define SF_DPIMM_LO 31
#define OPC_DPIMM_HI 30
#define OPC_DPIMM_LO 29
#define OPI_DPIMM_HI 25
#define OPI_DPIMM_LO 23
#define RD_DPIMM_HI 4
#define RD_DPIMM_LO 0

#define OPI_DPIMM_ARITH 2
#define RN_DPIMM_ARITH_HI 9
#define RN_DPIMM_ARITH_LO 5
#define SH_DPIMM_ARITH_HI 22
#define SH_DPIMM_ARITH_LO 22
#define IMM12_DPIMM_HI 21
#define IMM12_DPIMM_LO 10
#define DPIMM_ARITH_SHIFT_AMOUNT 12

#define OPI_DPIMM_WIDE_MOVE 5
#define SH_DPIMM_WM_HI 22
#define SH_DPIMM_WM_LO 21
#define IMM16_DPIMM_WM_HI 20
#define IMM16_DPIMM_WM_LO 5

#define DPIMM_WIDE_MOVE_SHIFT_FACTOR 16
#define OPC_DPIMM_MOVN 0
#define OPC_DPIMM_MOVZ 2
#define OPC_DPIMM_MOVK 3

#endif
