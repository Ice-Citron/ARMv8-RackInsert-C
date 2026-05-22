#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MEM_SIZE (1 << 21)
#define REGS 31


uint8_t memory[MEM_SIZE];
uint64_t registers[REGS];
uint64_t pc;
typedef struct {
    bool n;
    bool z;
    bool c;
    bool v;
} pState;

const uint64_t zeroRegister = 0;


int main(int argc, char **argv) {
  return EXIT_SUCCESS;
}
