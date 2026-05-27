#ifndef EMULATOR_LOADER_H
#define EMULATOR_LOADER_H

#include <stdint.h>

int load_program(const char *path, uint8_t memory[], size_t memory_size,
                 size_t *bytes_loaded);
uint32_t fetch_u32_le(const uint8_t memory[], uint64_t address);
void run_emulator(void);
void write_final_state(FILE *out);

#endif