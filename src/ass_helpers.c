#include "ass_helpers.h"

void print_error_and_exit(const char *complaint) {
    fprintf(stderr, "ERROR %s\n", complaint);
    exit(1);
}

static const char *skip_hash(const char *s) {
    return s[0] == '#' ? s + 1 : s;
}

uint32_t read_number_or_label(char *string) {
    uint32_t inputint;
    if (find_address_from_sym_table(string, &inputint)) {
        return inputint;
    }
    const char *text = skip_hash(string);
    char *endptr = NULL;
    unsigned long value = strtoul(text, &endptr, 0);
    // Check: no digits parsed, not at end, or overflow
    if (text == endptr || *endptr != '\0' || value > UINT32_MAX) {
        print_error_and_exit("INVALID NUMBER OR LABEL");
    }
    return (uint32_t)value;
}

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
        print_error_and_exit("INVALID REGISTER LABEL");
    }

    char *end = NULL;
    uint32_t reg = strtoul(text + 1, &end, 10);
    if (*end != '\0' || reg > ZERO_REGISTER_NUMBER - 1) {
        print_error_and_exit("INVALID REGISTER NUMBER");
    }

    return reg;
}

