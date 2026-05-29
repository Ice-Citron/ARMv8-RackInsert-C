#include "emulate.h"
#include "loader.h"

uint8_t memory[MEM_SIZE];
uint64_t registers[REGS];
uint64_t pc = 0;
state pState = INITIAL_PSTATE;

int main(void) {
    registers[31] = 0ULL;
    run_emulator();
    return EXIT_SUCCESS;
}