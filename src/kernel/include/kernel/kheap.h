/*Davide Morazzo - Settembre 2024

Implementazione per allocazione dinamica di memoria nel kernel

*/
#ifndef INCLUDE_KERNEL_MEMORY_H 
#define INCLUDE_KERNEL_MEMORY_H 1

#include <stddef.h>

void *kmalloc(size_t size);
void kfree (void *ptr, size_t size);

/*Initialize array of num elements of specified size
and set elements to zero. If succeed return pointer of array.*/
void calloc(size_t num, size_t size);

typedef struct __kFreeChunk{
	char status[8];
	size_t size;
	struct __kFreeChunk *prevChunk;
	struct __kFreeChunk *nextChunk;
} _kFreeChunk;

typedef struct {
	void *freeBase;
	void *freeTop;
	_kFreeChunk *freeChunk;
} _kAllocStatus;

/*base is the heap memory region start, size in bytes of the heap
memory region.*/
_kAllocStatus* _setupHeap(void *base, size_t size);

#endif