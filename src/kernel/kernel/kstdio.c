#include "kernel/kstdio.h"
#include "kernel/tty.h"
#include <stdarg.h>
#include "stdio.h"


/* Kernel print function, v0.1 only supports 
output on serial console */
void kprint(const char * format, ...){

    va_list argptr;             // List iterator
    va_start (argptr, format);  // Initialize iterator on format
    char out_buffer[300] = {'\0'};
    _va_snprintf(out_buffer, 0, format, argptr);
    serial_writestring(STDIO_PORT, out_buffer);
    return;
}

// void get_placeholder(char *placeholder, char identifier, type argptr){

// }