#include "first_pass_helpers.h"

bool check_in_string_array(const char *target, const char *arr[],
    const int size)
{
    for (int i = 0; i < size; i++)
    {
        if (strcmp(target, arr[i]) == 0)
        {
            return true;
        }
    }
    return false;
}

bool check_in_alias(const char *str)
{
    return check_in_string_array(str, aliases, ALIASES_SIZE);
}

bool check_in_conds(const char *str)
{
    if (str[0] != 'b')
    {
        return false;
    }
    if (strcmp(str, "b:") == 0 || strcmp(str, "br:") == 0)
    {
        return false;
    }
    return true;
}

bool check_in_instrs(const char *str)
{
    return check_in_string_array(str, instrs, INSTRS_SIZE);
}