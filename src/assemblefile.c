#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// bool ok = assemblefile( filename );
//	Takes the name of a .as file, opens it, assembles it,
//	producing either one or more error messages (and returning false)
//	or produces the .em (listing) file, and returns true.
// 

bool assemblefile(char *infile, char *outfile) {
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
	int numberlines;

    //make new symbol table
    (void)init_symtab();
	bool file_end = false;
	// Now read all lines from the open file and process them

	char full_line_buffer[512];

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