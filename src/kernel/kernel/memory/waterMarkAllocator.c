/* Davide Morazzo - Settembre 2024 

Implementazione di malloc e free con water mark allocator, per avere una implementazione semplice*/

#include "kernel/kheap.h"
#include "string.h"

_kAllocStatus *_heapStatus;

_kAllocStatus* _setupHeap(void *base, size_t size){

	_kAllocStatus *kHeapStatus = (_kAllocStatus *)base;
	// kHeapStatus->freeChunk[0].nextChunk = NULL;
	// kHeapStatus->freeChunk[0].prevChunk = NULL;
	// kHeapStatus->freeChunk[0].size = 0;
	kHeapStatus->freeChunk = NULL;

	/* Allocate kHeapStatus at the beginning of the heap */
	kHeapStatus->freeBase = (void *)((char *)base + sizeof(_kAllocStatus));
	kHeapStatus->freeTop = (void *)((char *)base + size);
	_heapStatus = kHeapStatus;

	return kHeapStatus;
}

void *kmalloc(size_t size){
	if ((char *)_heapStatus->freeBase + size < (char *)_heapStatus->freeTop)
	{
		void *memPtr = _heapStatus->freeBase;
		_heapStatus->freeBase = (void *)((char*)_heapStatus->freeBase+size);
		return memPtr;
	}
	else
	{
		// Heap full. return NULL
		// TODO: Handle freed chunks
		return NULL;
	}
}

void _kfree (void *ptr, size_t size){
	// if(ptr > _heapStatus->freeTop ){
	// 	return;
	// }

	// _kFreeChunk *chunk = kmalloc(sizeof(_kFreeChunk));
	// strcpy("F R E E", chunk->status);
    // chunk->start = ptr;
	// chunk->size = size;
	// chunk->nextChunk = NULL;

	// if(_heapStatus->freeChunk == NULL)
	// {
	// 	/*First free. Setup Chunk list head */
	// 	chunk->prevChunk = NULL;
	// 	_heapStatus->freeChunk = chunk;
	// }
	// else
	// {
	// 	// TODO: inserire nuovo oggetto nella lista in ordine
	// 	// secondo il puntatore liberato.
	// 	_kFreeChunk *c = _heapStatus->freeChunk;
	// 	while (c->nextChunk!=NULL){
	// 		c = c->nextChunk;
	// 	}
	// 	c->nextChunk = chunk;
	// 	chunk->prevChunk = c;
	// }
}

void mergeNextChunk(_kFreeChunk *thisChunk){
    _kFreeChunk *next = thisChunk->nextChunk;
    // Remove next from list and relink the list
    thisChunk->nextChunk = next->nextChunk;
    next->nextChunk->prevChunk = thisChunk;

    // Calculate new chunk size
    thisChunk->size += next->size;

    // Free next chunk heap
    kfree(next);
}

void mergeChunks(_kFreeChunk *head){
    _kFreeChunk *thisChunk = head;
    /* Iterate on the linked list */
    while (thisChunk->nextChunk != NULL){
        if ((thisChunk->start + thisChunk->size) == (thisChunk->nextChunk->start-1)){
            // Two consecutives chunks can be merged. Do not iterate next cycle to see if
            // this chunk can be merged again
            mergeNextChunk(thisChunk);
        }else{
            thisChunk = thisChunk->nextChunk;
        }
    }
}
