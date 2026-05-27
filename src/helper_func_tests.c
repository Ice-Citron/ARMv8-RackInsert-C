#include "emulate.h"
#include "loader.h"

int main (void) {
    /*
    printf("%s", "Expected answer for bitmask(7, 6): 0xc0\n");
    printf("0x%x", bitmask(7, 6));
    printf("%c", '\n');
    printf("%s", "Expected answer for bitmaskcheck(7, 6, 67): 0x1\n");
    printf("0x%x", bitmask_check(7, 6, 67));
    printf("%c", '\n');
    */
    printf("%zu", decode_and_execute(28, 25, 0x12FFFFFF))
}