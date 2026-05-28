#include "emulate.h"

#define DP_CLEAR_LOWER_32_MASK (~(uint64_t) UINT32_MAX)

#define DP_OPC_ADD 0
#define DP_OPC_ADD_SETFLAG 1
#define DP_OPC_SUBTRACT_START 2
#define DP_OPC_SUB_SETFLAG 3

#define DP_16BIT_MASK UINT16_MAX

static uint32_t get32from64(uint64_t target) {return (uint32_t) target;}
static uint64_t mask_from_sf(uint32_t sf) {return sf == 0 ? UINT32_MAX : UINT64_MAX;}