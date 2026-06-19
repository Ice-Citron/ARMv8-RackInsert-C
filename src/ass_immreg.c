#include "ass_immreg.h"
//decides index where we check whether 
//function is register or immediate
static idx_operand_no immreg_instrs[] = {
    {"add", 2},
    {"adds", 2},
    {"sub", 2},
    {"subs", 2},
    {"cmp", 1},
    {"cmn", 1},
    {"neg", 1},
    {"negs", 1},
};

opcount_checker immreg_opcounts[] = {
	{"cmpcmn", 2, 4},
	{"negnegs", 2, 4},
	{"other", 3, 5},
};

void check_opcode (char* mnemonic, uint32_t *opc) {
	if (strcmp(mnemonic,"add") == 0) {
        *opc = OPC_ADD;
    } else if (strcmp(mnemonic, "adds") == 0 || strcmp(mnemonic, "cmn") == 0) {
        *opc = OPC_ADDS;
    } else if (strcmp(mnemonic, "sub") == 0 || strcmp(mnemonic, "neg") == 0) {
        *opc = OPC_SUB;
    } else {
        *opc = OPC_SUBS;
    }
    *opc = *opc << DP_OPC_SHIFT;
}

//function handling decision whether immediate or register for dpimm and dpreg instrs
uint32_t decide_imm_or_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {
    size_t operand_2_idx = 0;
	bool found_instruction = false;

    for (size_t i = 0; i < IMMREG_INSTR_COUNT; i++) {
        if (strcmp(mnemonic, immreg_instrs[i].instrname) == 0) {
            operand_2_idx = immreg_instrs[i].index;
			found_instruction = true;
			break;
        }
	}

	if (!found_instruction) {
		print_error_and_exit("UNKNOWN MNEMONIC");
	}

	if (operand_count < operand_2_idx) {
		print_error_and_exit("TOO FEW OPERANDS");
	}

	if (operands[operand_2_idx] [0] == IMMEDIATE_CHARACTER) {
		return assemble_dp_imm(mnemonic, operands, operand_count, pc);
	}
	
	return assemble_dp_reg(mnemonic, operands, operand_count, pc);
}