#ifndef ASSEMBLE_FILE_H
#define ASSEMBLE_FILE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

bool assemble_file(char *infile, char *outfile);

#endif // ASSEMBLE_FILE_H
