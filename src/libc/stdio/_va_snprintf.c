#include <stdarg.h>
#include "stdio.h"
#include "string.h"

int _va_snprintf(char * s, size_t n, const char * format, va_list argptr){
    char out_buffer[300] = {'\0'};   // TODO: make dynamic
    char placeholder[50] = {'\0'};
    size_t fmt_size = strlen(format);

    for(size_t i=0; i<fmt_size; i++){
        
        // Iterate on string to find placeholders
        if (format[i] == '%' && i != fmt_size-1){
            // Placeholder found and not last iteration
            switch (format[i+1])
            {
            case 'X': /*Print hex*/
                i32tohex(*va_arg(argptr, uint32_t*), placeholder);
                break;
            case 'd': /*Print integer*/
                itoa(*va_arg(argptr,int*), placeholder);
                break;
            case 's': /*Print string*/
                strcpy(va_arg(argptr, char*), placeholder);
                break;
            default:
                i--;
                break;
            }
            // get_placeholder(placeholder, format[i+1], argptr);
            strcpy(placeholder, &out_buffer[strlen(out_buffer)]);
            i++;
        }else{
            // No placeholder. Append char to output buffer
            out_buffer[strlen(out_buffer)] = format[i];
        }
    }
    
    if (n != 0){
        strcpyn(out_buffer, s, n);
    }else{
        strcpy(out_buffer, s);
    }
    va_end(argptr);             // Clean-up
    return n;   // TODO: da rendere robusto
}