#include <loader.h>

#include <emulate.h>
#include <stdint.h>


int load_program(const char *path, uint8_t memory[], size_t *bytes_loaded) {
    if (file)
    
    FILE* file = fopen(path, "r");
    if (file == null) {
        perror("ERROR: Failed to open file at path: %s\n", path);
        return 0;
    }

    // Initially, try reading MEM_SIZE bytes. If file is bigger than MEM_SIZE,
    // then second fread() will > 0.
    size_t bytes_read = fread(memory, sizeof(*memory), MEM_SIZE, file);

    unsigned char extra;
    if (fread(&extra, 1, 1, file) > 0) {
        perror("ERROR: Unable to fully load program into RAM, as program size
                > RAM size.\n");
        fclose(file);
        return 0;
    }

    fclose(file);
    *bytes_loaded = bytes_read;
    
    return 1;
}

uint32_t fetch_u32_le(const uint8_t memory[], uint64_t address) {

}

void run_emulator(void) {

}

void write_final_state(FILE *out) {

}