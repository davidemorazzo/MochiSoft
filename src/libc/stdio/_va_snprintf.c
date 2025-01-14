#include <stdarg.h>
#include "stdio.h"
#include "string.h"

int _va_snprintf(char * s, size_t n, const char * format, va_list argptr){
    size_t fmt_size = strlen(format);
    unsigned int s_ptr = 0;

    for(size_t i=0; i<fmt_size; i++){
        
        // Iterate on string to find placeholders
        if (format[i] == '%' && i != fmt_size-1){
            // Placeholder found and not last iteration
            switch (format[i+1])
            {
            case 'X': /*Print hex*/
                i32tohex(va_arg(argptr, uint32_t), &s[s_ptr]);
                s_ptr += 8;
                break;
            case 'd': /*Print integer*/
                itoa(va_arg(argptr,int), &s[s_ptr]);
                s_ptr += strlen(&s[s_ptr]);
                break;
            case 's': /*Print string*/
                strcpy(va_arg(argptr, char*), &s[s_ptr]);
                s_ptr += strlen(&s[s_ptr]);
                break;
            default:
                i--;
                break;
            }

            i++;
        }else{
            // No placeholder. Append char to output buffer
            s[s_ptr++] = format[i];

        }
        if (n != 0){
            return s_ptr+1;
        }
    }
    
    va_end(argptr);             // Clean-up
    s[s_ptr] = '\0'; 
    return s_ptr+1;
}