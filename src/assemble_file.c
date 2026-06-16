#include "assemble_file.h"

#include "assemble_dp.h"
#define MAX_OPERANDS 5

bool assemble_file(char *infile, char *outfile) {
    // open the input file
	FILE *in = fopen(infile, "r" );
	if( in == NULL )
	{
		fprintf(stderr, "ass: can't open %s\n", infile);
		exit(1);
	}
    // open the output file
    FILE *out = fopen(outfile, "w" );
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
		instruction = assemble_dp(mnemonic, operands, operand_count);
		fwrite(&instruction, sizeof(uint32_t), 1, out);
		pc += 4;
	}
	fclose(in);
	fclose(out);
    //true if no errors, false if have errors
    return true;
}