#include "string.h"

char *trim(char *s)
{
    return rtrim(ltrim(s)); 
}