#include <stdarg.h>
#include "stdio.h"
#include "string.h"

int sprintf ( char * str, const char * format, ... ){
    va_list argptr;
    va_start(argptr, format);
    
    return _va_snprintf(str, 0, format, argptr);
}