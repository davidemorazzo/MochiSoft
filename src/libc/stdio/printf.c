/* Novembre 2024 - Davide Morazzo */

#include "stdio.h"
#include "string.h"
#include <stdint.h>

int printf (const char * format, ... ){
	va_list argptr;
	va_start(argptr, format);

	char buffer[300]; 
	uint32_t *buf_addr = (uint32_t*)buffer;
	_va_snprintf(buffer, 0, format, argptr);
	int buf_size = strlen(buffer);

	/*Make syscall*/	
	uint32_t res = 0;
    __asm__(
        "mov $1, %%eax\n\t"		// Syscall ID
        "mov $1, %%ebx\n\t"		// stdout
        "mov %1, %%ecx\n\t"		// buffer
        "mov %2, %%edx\n\t"		// size
        "int $0x80\n\t"
        "mov %%eax, %0"			// result
        : "=m" (res)
        : "m" (buf_addr), "m" (buf_size)
    );

	return res;
}