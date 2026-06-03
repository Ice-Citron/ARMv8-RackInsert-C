#ifndef DATA_TRANSFER_H
#define DATA_TRANSFER_H

#include "emulate.h"
#include "bit_manipulation.h"

#define DT_UINT_IMM_OFFSET 1
#define DT_PRE_POST_INDEX 1
#define DT_PRE_IDX 1
#define DT_32BIT_MODE 0
#define DT_LOAD_OP 1
#define DT_SINGLE_MODE 1
#define DT_REG_OFFSET_MODE 26

#define DT_RT_HI 4
#define DT_RT_LO 0
#define DT_SIZE_HI 30
#define DT_SIZE_LO 30
#define DT_XN_HI 9
#define DT_XN_LO 5
#define DT_OP_HI 22
#define DT_OP_LO 22
#define DT_SINGLE_HI 31
#define DT_SINGLE_LO 31
#define DT_UINT_HI 24
#define DT_UINT_LO 24
#define DT_IMM12_HI 21
#define DT_IMM12_LO 10
#define DT_SIMM9_HI 20
#define DT_SIMM9_LO 12
#define DT_PINDEX_HI 11
#define DT_PINDEX_LO 11
#define DT_PRE_POST_HI 10
#define DT_PRE_POST_LO 10
#define DT_OFFSET_HI 15
#define DT_OFFSET_LO 10
#define DT_XM_HI 20
#define DT_XM_LO 16
#define DT_LOAD_HI 23
#define DT_LOAD_LO 5

void single_data_transfer(const uint32_t instr);

#endif
