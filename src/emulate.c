#include "emulate.h"

uint8_t memory[MEM_SIZE];
uint64_t registers[REGS];
uint64_t pc = 0;
state pState = { false, false, false, false };

int main(void) {
    registers[31] = 0ULL;
    // printf("%zu", bitmask_check(28, 25, 0x12FFFFFF));
    return EXIT_SUCCESS;
}