#include <stdbool.h>
#include <string.h>
#define CONDS_SIZE 7
#define ALIASES_SIZE 8
#define INSTRS_SIZE 19

// temporary solution for identifying branches
static char *conds[CONDS_SIZE] = {"eq", "ne", "ge", "lt", "gt", "le", "al"};
static char *aliases[ALIASES_SIZE] = {"cmp", "cmn", "neg(s)", "tst", "mvn",
    "mov", "mul", "mneg"};
static char *instrs[INSTRS_SIZE] = {"add(s)", "sub(s)", "and(s)", "bic(s)",
    "eor", "eon", "orr", "orn", "movn", "movk", "movz", "madd", "msub", "b",
    "br", "b.cond", "ldr", "str", "and"};