#include <stdarg.h>
#include "stdio.h"
#include "string.h"

int snprintf ( char * s, size_t n, const char * format, ... ){
    va_list argptr;
    va_start(argptr, format);

    return _va_snprintf(s, n, format, argptr); 
}