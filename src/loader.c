#include "loader.h"
#include "emulate.h"
#include "dp_shared.h"
#include "data_transfer.h"
#include "branch.h"

bool load_program(const char *path, size_t *bytes_loaded) {
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

uint32_t fetch_u32_le(uint64_t address) {
    // Since instructions are stored in little-endian format.
    return ((uint32_t)memory[address + 3] << 24)
         | ((uint32_t)memory[address + 2] << 16)
         | ((uint32_t)memory[address + 1] << 8)
         | ((uint32_t)memory[address + 0] << 0);
}

bool decode_and_execute(uint32_t instr) {
    switch (extract_bits(28, 25, instr)) {
        // 100x: Data Processing (Immediate)
        case 0x08:
        case 0x09:
            dpimm(instr);
            return false;
        // x101: Data Processing (Register)
        case 0x05:
        case 0x0d:
            dpreg(instr);
            return false;
        // x1x0: Loads and Stores
        case 0x04:
        case 0x06:
        case 0x0c:
        case 0x0e:
            single_data_transfer(instr);
            return false;
        case 0x0a:
        case 0x0b:
            return execute_branch(instr);
        default:
            fprintf(stderr, "ERROR: Unrecognised instruction type!\n");
            return false;
    }
}

void run_emulator(void) {
    while (true) {
        uint32_t curr_instruction = fetch_u32_le(memory, pc);
        if (curr_instruction == HALT_INSTRUCTION) {
            break;
        }
        bool pc_changed = decode_and_execute(curr_instruction);
        if (!pc_changed) {
            pc += 4;
        }
    }
}

void write_final_state(FILE *file) {
    fprintf(file, "Registers:\n");
    for (int i = 0; i < REGS - 1; i++) {
        fprintf(file, "X%02d    = %016"PRIx64"\n", i, registers[i]);
    }
    fprintf(file, "PC     = %016"PRIx64"\n", pc);
    fprintf(file, "Non-zero memory: \n");
    for (int i = 0; i <= MEM_SIZE - 4; i += 4) {
        if (memory[i] != 0) {
            fprintf(file, "0x%08"PRIx16": 0x%08"PRIx16"\n", (uint16_t)(i * 4), 
                    fetch_u32_le(i));
        }
    }
    fclose(file);
}