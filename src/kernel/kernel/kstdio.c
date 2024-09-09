#include "kernel/kstdio.h"
#include "kernel/tty.h"
#include "string.h"
#include <stdarg.h>


/* Kernel print function, v0.1 only supports 
output on serial console */
void kprint(const char * format, ...){

    va_list argptr;             // List iterator
    va_start (argptr, format);  // Initialize iterator on format
    char out_buffer[300] = {'\0'};
    char placeholder[50];
    size_t fmt_size = strlen(format);

    for(size_t i=0; i<fmt_size; i++){
        
        // Iterate on string to find placeholders
        if (format[i] == '%' && i != fmt_size-1){
            // Placeholder found and not last iteration
            switch (format[i+1])
            {
            case 'X':
                i32tohex(*va_arg(argptr, uint32_t*), placeholder);
                break;
            default:
                i--;
                break;
            }
            // get_placeholder(placeholder, format[i+1], argptr);
            strcpy(placeholder, &out_buffer[strlen(out_buffer)], strlen(placeholder));
            i++;
        }else{
            // No placeholder. Append char to output buffer
            out_buffer[strlen(out_buffer)] = format[i];
        }
    }
    
    serial_writestring(STDIO_PORT, out_buffer);
    va_end(argptr);             // Clean-up
    return;
}

// void get_placeholder(char *placeholder, char identifier, type argptr){

// }