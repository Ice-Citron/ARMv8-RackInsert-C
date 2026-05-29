#ifndef SRC_BIT_MANIPULATION_H
#define SRC_BIT_MANIPULATION_H

#include <stdint.h>
#include "emulate.h"

static uint32_t bitmask (const uint8_t start, const uint8_t end) {
    //generates the bitmask which starts at start and ends at end
    const uint8_t width = start - end + 1;

    if (width == 32) {
        return UINT32_MAX;
    }
    return ((1u << width) - 1u) << end;
}

static uint32_t extract_bits (const uint8_t start, const uint8_t end, 
                              const uint64_t target) {
    return (bitmask(start, end) & target) >> end;
}

static long long get_signed_value_from_bits(
    const uint8_t start,
    const uint8_t end,
    const uint32_t target)
{
    const uint8_t width = start - end + 1;
    const uint32_t unsigned_val = extract_bits(start, end, target);
    long long val = (long long)unsigned_val;
    if ((unsigned_val >> (width - 1)) & 1)
    {
        val -= (unsigned_val >> (width - 1)) << width;
    }
    return val;
}

#endif //SRC_BIT_MANIPULATION_H
