#include "parse_reg.h"

uint32_t parse_reg(const char *text, uint32_t *sf) {
    if (strcmp(text, "xzr") == 0) {
        *sf = REG_X_SF;
        return ZERO_REGISTER_NUMBER;
    }

    if (strcmp(text, "wzr") == 0) {
        *sf = REG_W_SF;
        return ZERO_REGISTER_NUMBER;
    }

    if (text[0] == 'x') {
        *sf = REG_X_SF;
    } else if (text[0] == 'w') {
        *sf = REG_W_SF;
    } else {
        fprintf(stderr, "invalid register: %s\n", text);
        exit(1);
    }

    char *end = NULL;
    uint32_t reg = strtoul(text + 1, &end, 10);
    if (*end != '\0' || reg > 30) {
        fprintf(stderr, "invalid register: %s\n", text);
        exit(1);
    }

    return reg;
}