#include "first_pass_helpers.h"

bool check_in_alias(const char *str)
{
    for (int i=0; i<ALIASES_SIZE; i++)
    {
        if (strcmp(str, aliases[i]) == 0)
        {
            return true;
        }
    }
    return false;
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
    for (int i=0; i<INSTRS_SIZE; i++)
    {
        if (strcmp(str, instrs[i]) == 0)
        {
            return true;
        }
    }
    return false;
}