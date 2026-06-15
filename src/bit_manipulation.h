#ifndef SRC_BIT_MANIPULATION_H
#define SRC_BIT_MANIPULATION_H

#include <stdint.h>

/*
 * (Helper) Creates a mask covering bits start down to end.
 * Example: bitmask(3, 1) returns 0b1110.
 */
static inline uint32_t bitmask(const uint8_t start, const uint8_t end) {
    const uint8_t width = start - end + 1;
    if (width == 32) {
        return UINT32_MAX;
    }
    return ((1u << width) - 1u) << end;
}

/*
 * (Helper) Extracts bits start down to end from a value.
 * Example: extract_bits(3, 1, 0b10110) returns 0b011.
 */
static inline uint32_t extract_bits(const uint8_t start, const uint8_t end, 
                                    const uint64_t target) {
    return (bitmask(start, end) & target) >> end;
}

/*
 * (Helper) Extracts a signed bit field and sign-extends it.
 * Example: a 6-bit value 0b111111 is returned as -1.
 */
static inline long long get_signed_value(const uint8_t start,
                                         const uint8_t end,
                                         const uint32_t target) {
    const uint8_t width = start - end + 1;
    const uint32_t unsigned_val = extract_bits(start, end, target);
    long long val = (long long)unsigned_val;
    if ((unsigned_val >> (width - 1)) & 1) {
        val -= 1LL << width;    // turns unsigned value into signed equivalent
    }
    return val;
}

#endif
