//
// Created by Rayan Abdallah on 28/05/2026.
//

#ifndef SRC_BIT_MANIPULATION_H
#define SRC_BIT_MANIPULATION_H
#include <stdint.h>

static inline uint32_t bitmask (const uint8_t start, const uint8_t end) {
    //generates the bitmask which starts at start and ends at end
    uint8_t width = start - end + 1;

    if (width == 32) {
        return UINT32_MAX;
    }
    return ((1u << width) - 1u) << end;
}

static inline uint32_t extract_bits (const uint8_t start, const uint8_t end, const uint32_t target) {
    return (bitmask(start, end) & target) >> end;
}


#endif //SRC_BIT_MANIPULATION_H
