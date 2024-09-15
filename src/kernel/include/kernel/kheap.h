/*Davide Morazzo - Settembre 2024

Implementazione per allocazione dinamica di memoria nel kernel
*/
#ifndef INCLUDE_KERNEL_MEMORY_H 
#define INCLUDE_KERNEL_MEMORY_H 1

#include <stddef.h>

void *kmalloc(size_t size);
void kfree (void *ptr);

/*Initialize array of num elements of specified size
and set elements to zero. If succeed return pointer of array.*/
void calloc(size_t num, size_t size);

#endif