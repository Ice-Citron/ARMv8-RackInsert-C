#include "emulate.h"
#include "loader.h"

uint8_t memory[MEM_SIZE];
uint64_t registers[REGS];
uint64_t pc = 0;
state pState = INITIAL_PSTATE;

int main(int argc, char *argv[]) {
    if (argc != 2 && argc != 3) {
        fprintf(stderr, "ERROR: Invalid arguments passed. 'emulate' requires"
                " format of: ./emulate <file_in> or ./emulate <file_in> "
                "<file_out>\n");
        return -1;
    }

    size_t bytes_loaded;
    bool load_success = load_program(argv[1], &bytes_loaded);
    if (!load_success) {
        fprintf(stderr, "ERROR: Failed to load program from %s\n", argv[1]);
        return -1;
    } 
    registers[31] = 0ULL;
    run_emulator();

    if (argc == 2) {
        // Without output file, emulator prints to stdout
        write_final_state(stdout);
    } else if (argc == 3) {
        // Has output file, emulator prints to specified path
        FILE *file = fopen(argv[2], "wb");
        if (file == NULL) {
            printf(stderr, "ERROR: Unable to write final output state to file "
                   "in %s\n", argv[2]);
            return -1;
        }
        write_final_state(file);
        fclose(file);
    }
    
    return EXIT_SUCCESS;
}