#include "loader.h"
#include "dp_shared.h"
#include "data_transfer.h"
#include "branch.h"

/*
 * Loads the input binary file into emulator's memory.
 * Example: given a 16-byte binary, loads 16 bytes into memory and sets 
 * *bytes_loaded to 16.
 */
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

/*
 * Fetches one 32-bit instruction from memory in little-endian order.
 * Example: bytes 78 56 34 12 at address 0 returns 0x12345678.
 */
uint32_t fetch_u32_le(uint64_t address) {
    // Since instructions are stored in little-endian format.
    return ((uint32_t)memory[address + 3] << 24)
         | ((uint32_t)memory[address + 2] << 16)
         | ((uint32_t)memory[address + 1] << 8)
         | ((uint32_t)memory[address + 0] << 0);
}

/*
 * Decodes an instruction's opcode and dispatches it to relevant instruction
 * handler. Returns true if the handler changed PC (for run_emulator() function)
 * Example: a branch instruction is sent to execute_branch and may return true.
 */
static bool decode_and_execute(uint32_t instr) {
    switch (extract_bits(OP0_HI, OP0_LO, instr)) {
        case OP0_DP_IMM_1000:
        case OP0_DP_IMM_1001:
            dpimm(instr);
            return false;
        case OP0_DP_REG_0101:
        case OP0_DP_REG_1101:
            dpreg(instr);
            return false;
        case OP0_LOAD_STORE_0100:
        case OP0_LOAD_STORE_0110:
        case OP0_LOAD_STORE_1100:
        case OP0_LOAD_STORE_1110:
            single_data_transfer(instr);
            return false;
        case OP0_BRANCH_1010:
        case OP0_BRANCH_1011:
            return execute_branch(instr);
        default:
            fprintf(stderr, "ERROR: Unrecognised instruction type!\n");
            exit(EXIT_FAILURE);
    }
}

/*
 * Runs  emulator until halt instruction (HALT_INSTRUCTION) is fetched.
 * Example: if memory[pc] is HALT_INSTRUCTION, the loop stops immediately.
 */
void run_emulator(void) {
    while (true) {
        uint32_t curr_instruction = fetch_u32_le(pc);
        if (curr_instruction == HALT_INSTRUCTION) {
            break;
        }
        bool pc_changed = decode_and_execute(curr_instruction);
        if (!pc_changed) {
            pc += INSTRUCTION_BYTES;
        }
    }
}

/*
 * Writes the final register, PC, PSTATE and non-zero memory state into 
 * designated `FILE *file` path.
 * Example: write_final_state(stdout) prints the emulator state to terminal.
 */
void write_final_state(FILE *file) {
    fprintf(file, "Registers:\n");
    for (int i = 0; i < REGS - 1; i++) {
        fprintf(file, "X%02d    = %016"PRIx64"\n", i, registers[i]);
    }
    fprintf(file, "PC     = %016"PRIx64"\n", pc);
    char n = (pState.n == true) ? 'N' : '-';
    char z = (pState.z == true) ? 'Z' : '-';
    char c = (pState.c == true) ? 'C' : '-';
    char w = (pState.v == true) ? 'V' : '-';
    fprintf(file, "PSTATE : %c%c%c%c\n", n, z, c, w);
    fprintf(file, "Non-zero memory: \n");
    for (int i = 0; i <= MEM_SIZE - WORD_BYTES; i += WORD_BYTES) {
        uint32_t word = fetch_u32_le((uint64_t)i);
        if (word != 0) {
            fprintf(file, "0x%08"PRIx64": %08"PRIx32"\n", (uint64_t)i, word);
        }
    }
}
