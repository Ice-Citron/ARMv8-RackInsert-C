#include "emulate.h"

uint8_t memory[MEM_SIZE] = malloc(MEM_SIZE * sizeof(*memory));
uint64_t registers[REGS] = malloc(REGS * sizeof(*registers));
uint64_t pc = 0;
state pState = {false, false, false, false};

int main(int argc, char **argv) {
    registers[31] = 0ULL;
    return EXIT_SUCCESS;
}