/* Maggio 2025 - Davide Morazzo 		*/
/* ------------------------------------	*/
/* Definizioni per i registri specifici */
/* Per CPU Intel 80386 (i386)			*/

#ifndef INCLUDE_ARCH_I386_REGISTERS_H
#define INCLUDE_ARCH_I386_REGISTERS_H 

#include <stdint.h>

typedef struct registers_s{
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t esp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;
}registers_t;

typedef struct eflags_s{
	uint32_t eflags;
}eflags_t;

#endif