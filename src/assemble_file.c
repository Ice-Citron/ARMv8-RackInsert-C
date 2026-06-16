#include "assemble_file.h"
#define MAX_OPERANDS 5

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

//function handling decision whether immediate or register for dpimm and dpreg instrs
static uint32_t decide_imm_or_reg(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {
    size_t operand_2_idx;
    for (int i = 0; i < IMMREG_INSTR_COUNT; i++) {
        if (strcmp(mnemonic, immreg_instrs[i].instrname) == 0) {
            operand_2_idx = immreg_instrs[i].index;
        }
        if (operands[operand_2_idx] [0] == IMMEDIATE_CHARACTER) {
            return assemble_dp_imm(mnemonic, operands, operand_count, pc);
        }
        else {
            return assemble_dp_reg(mnemonic, operands, operand_count, pc);
        }
    }
    fprintf(stderr, "function name doesnt exist");
    exit(1);
}

//dispatch table for app dp instructions
static instr_dispatch dp_instrs[] = {
    //3 assemble wide move
    {"movn", assemble_wide_move},
    {"movz", assemble_wide_move},
    {"movk", assemble_wide_move},
    //4 assemble multiply
    {"madd", assemble_multiply},
    {"msub", assemble_multiply},
    {"mul", assemble_multiply},
    {"mneg", assemble_multiply},
    //10 logical reg
    {"and", assemble_dp_logical_reg},
    {"bic", assemble_dp_logical_reg},
    {"orr", assemble_dp_logical_reg},
    {"eor", assemble_dp_logical_reg},
    {"eon", assemble_dp_logical_reg},
    {"ands", assemble_dp_logical_reg},
    {"bics", assemble_dp_logical_reg},
    {"tst", assemble_dp_logical_reg},
    {"mov", assemble_dp_logical_reg},
    {"mvn", assemble_dp_logical_reg},
    //8 decide imm or reg
    {"add", decide_imm_or_reg},
    {"adds", decide_imm_or_reg},
    {"sub", decide_imm_or_reg},
    {"subs", decide_imm_or_reg},
    {"cmp", decide_imm_or_reg},
    {"cmn", decide_imm_or_reg},
    {"neg", decide_imm_or_reg},
    {"negs", decide_imm_or_reg},
	//2 load/store
	{"ldr", ass_single_data_transfer}, 
	{"str", ass_single_data_transfer},
	//3 branch 
	{"b", ass_branch}, 
	{"br", ass_branch},
	{"b.cond", ass_branch},
	//1 directive
	{".int", int_directive}
};

//main dp assembling function
uint32_t assemble_instructions(char* mnemonic, char *operands[], size_t operand_count, uint32_t pc) {
    for (int i = 0; i < INSTR_COUNT; i++) {
        if (strcmp(mnemonic, dp_instrs[i].instrname) == 0) {
            return dp_instrs[i].handler(mnemonic, operands, operand_count, pc);
        }
    }
    fprintf(stderr, "function name doesnt exist");
    exit(1);
}

bool assemble_file(char *infile, char *outfile) {
    // open the input file
	FILE *in = fopen(infile, "r" );
	if( in == NULL )
	{
		fprintf(stderr, "ass: can't open %s\n", infile);
		exit(1);
	}
    // open the output file
    FILE *out = fopen(outfile, "wb" );
	if( out == NULL )
	{
		fprintf( stderr, "ass: can't create %s\n", outfile );
		exit(1);
	}

	// FIRST PASS:

	uint32_t pc = 0;

	char full_line_buffer[512];
	while(fgets(full_line_buffer, sizeof(full_line_buffer), in) != NULL) {
		char* inner_save_ptr = NULL;
		char* starting_query = strtok_r(full_line_buffer, " \t\n",
			&inner_save_ptr);
		if (starting_query == NULL) // in case of extra lines
		{
			continue;
		}

		const size_t len = strlen(starting_query);

		if (starting_query[len-1] == ':') // if last char is : then it's a label
		{
			starting_query[len-1] = '\0';
			add_to_symbol_table(starting_query, pc);
		}
		else
		{
			pc+=4; // 4 bytes increase on pc
		}
	}

	// bool file_end = false;

	rewind(in);
	pc = 0;

	//  SECOND PASS:
	// Now read all lines from the open file and process them

	while(fgets(full_line_buffer, sizeof(full_line_buffer), in) != NULL) {
		char* inner_save_ptr = NULL;
		// char* starting_query = strtok_r(full_line_buffer, " \t\n",
		// 	&inner_save_ptr);
		char *mnemonic = strtok_r(full_line_buffer, " \t\n", &inner_save_ptr);
		if (mnemonic == NULL) // empty line
		{
			continue;
		}
		size_t len = strlen(mnemonic);
		if (mnemonic[len-1] == ':') // second pass so skip labels
		{
			continue;
		}
		char *operands[MAX_OPERANDS]; // store arguments here
		size_t operand_count = 0;
		char* args_of_query = strtok_r(NULL, ", \t\n", &inner_save_ptr);
		while(args_of_query != NULL)
		{
			operands[operand_count++] = args_of_query;
			args_of_query = strtok_r(NULL, ", \t\n", &inner_save_ptr);
		}
		uint32_t instruction = 0;
		instruction = assemble_instructions(mnemonic, operands, operand_count, pc);
		//add the other instructions - done
		fwrite(&instruction, sizeof(uint32_t), 1, out);
		pc += 4;
	}
	fclose(in);
	fclose(out);
    //true if no errors, false if have errors
    return true;
}