#include "string.h"

char *ltrim(char *s)
{
    while(isspace(*s)) s++;
    return s;
}