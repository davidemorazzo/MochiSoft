#include "kernel/kstdio.h"
#include <stdarg.h>
#include "stdio.h"
#include "kernel/syscall.h"
#include "string.h"

/* Kernel print function, v0.1 only supports 
output on serial console */
void kprint(const char * format, ...){

    va_list argptr;             // List iterator
    va_start (argptr, format);  // Initialize iterator on format
    char out_buffer[300] = {'\0'};
    _va_snprintf(out_buffer, 0, format, argptr);
    sys_write(stdout, out_buffer, strlen(out_buffer));
    return;
}

// void get_placeholder(char *placeholder, char identifier, type argptr){

// }