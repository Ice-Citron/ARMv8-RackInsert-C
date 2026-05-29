#include "emulate.h"
#include "loader.h"

uint8_t memory[MEM_SIZE];
uint64_t registers[REGS];
uint64_t pc = 0;
state pState = INITIAL_PSTATE;

int main(int argc, char *argv[]) {
    if (argc < 1 || argc > 2) {
        fprintf(stderr, "ERROR: Invalid arguments passed. 'emulate' requires"
                " format of: ./emulate <file_in> or ./emulate <file_in> "
                "<file_out>\n");
        return -1;
    }

    size_t bytes_loaded;
    bool load_success = load_program(argv[0], bytes_loaded);
    if (load_success) {
        printf("%zu bytes successfully loaded onto program from %s\n", 
               bytes_loaded, argv[0]);
    } else {
        fprintf(stderr, "ERROR: Failed to load program from %s\n", argv[0]);
        return -1;
    } 
    registers[31] = 0ULL;
    run_emulator();

    if (argc == 1) {
        // Without output file, emulator prints to stdout
        write_final_state(stdout);
    } else if (argc == 2) {
        // Has output file, emulator prints to specified path
        FILE *file = fopen(, "wb");
        if (file == NULL) {
            printf(stderr, "ERROR: Unable to write final output state to file "
                   "in %s\n", argv[1]);
            return -1;
        }
        write_final_state();
    }
    
    return EXIT_SUCCESS;
}