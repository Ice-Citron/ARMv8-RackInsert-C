#include <stdbool.h>
#include <stdio.h>
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
	char* curr_line = strtok(infile, "\n");
	while(!file_end) {
		curr_line = strtok(NULL, "\n");
		//process current line and write file
		if (curr_line == NULL) {
			file_end = true;
		}
	}
	fclose(in);
    //true if no errors, false if have errors
    return true;
}