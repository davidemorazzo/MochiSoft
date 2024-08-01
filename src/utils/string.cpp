#ifndef STRING
#define STRING

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

size_t strlen (const char* str){
    size_t size = 0;
    while (str[size])
        size++;
    return size;
}

#endif