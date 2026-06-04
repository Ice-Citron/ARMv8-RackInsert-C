#include "assemble_file.h"
// bool ok = assemblefile( filename );
//	Takes the name of a .as file, opens it, assembles it,
//	producing either one or more error messages (and returning false)
//	or produces the .em (listing) file, and returns true.
//

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

	int pc = 0;

	char full_line_buffer[512];
	while(fgets(full_line_buffer, sizeof(full_line_buffer), in) != NULL) {
		char* inner_save_ptr = NULL;
		const char* starting_query = strtok_r(full_line_buffer, " \t\n",
			&inner_save_ptr);
		if (!check_in_alias(starting_query) && !check_in_conds(starting_query)
			&& !check_in_instrs(starting_query))
		{
			add_to_symbol_table(starting_query, pc);
		}
		pc += 4;
	}

	bool file_end = false;

	//  SECOND PASS:
	// Now read all lines from the open file and process them

	while(fgets(full_line_buffer, sizeof(full_line_buffer), in) != NULL) {
		char* inner_save_ptr = NULL;
		char* starting_query = strtok_r(full_line_buffer, " \t\n",
			&inner_save_ptr);
		// dictate what to do from here
		char* args_of_query = strtok_r(NULL, ", \t\n", &inner_save_ptr);
		while(args_of_query != NULL)
		{
			// do what I must here
			args_of_query = strtok_r(NULL, ", \t\n", &inner_save_ptr);
		}
	}
	fclose(in);
	fclose(out);
    //true if no errors, false if have errors
    return true;
}