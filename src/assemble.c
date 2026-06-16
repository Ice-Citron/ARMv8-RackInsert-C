#include "assemble.h"

int main(int argc, char **argv) {
  if (argc != MAIN_NUMBEROF_ARGUMENTS) {
    fprintf(stderr, "Syntax error: ./assemble <input file> <output file>");
    exit(1);
  }
  assemble_file(argv[0], argv[1]);
  return EXIT_SUCCESS;
}
