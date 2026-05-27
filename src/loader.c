#include "loader.h"
#include "emulate.h"


bool load_program(const char *path, uint8_t memory[], size_t *bytes_loaded) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        fprintf(stderr, "ERROR: Failed to open file at path: %s\n", path);
        return false;
    }

    // Initially, try reading MEM_SIZE bytes. If file is bigger than MEM_SIZE,
    // then second fread() will > 0.
    size_t bytes_read = fread(memory, sizeof(*memory), MEM_SIZE, file);

    unsigned char extra;
    // Program File size > RAM size. Error!
    if (fread(&extra, 1, 1, file) > 0) {
        fprintf(stderr, "ERROR: Unable to fully load program into RAM, as "
                        "Program File size > RAM size.\n");
        fclose(file);
        return false;
    }

    fclose(file);   // fclose to free up file-pointer to prevent memory leak.
    *bytes_loaded = bytes_read;
    return true;
}

uint32_t fetch_u32_le(const uint8_t memory[], uint64_t address) {
    return ((uint32_t)memory[address + 3] << 24)
         | ((uint32_t)memory[address + 2] << 16)
         | ((uint32_t)memory[address + 1] << 8)
         | ((uint32_t)memory[address + 0] << 0);
}

void decode_and_execute(uint32_t instr) {
    switch (bitmask_check(28, 25, instr)) {
        case :
    }
}

void run_emulator(void) {

    while (true) {
        uint32_t curr_instruction = fetch_u32_le(memory, pc);
        if (curr_instruction == HALT_INSTRUCTION) {
            break;
        }
        // uint32_t old_pc;
        pc += 4;
    }
}

void write_final_state(FILE *out) {

}