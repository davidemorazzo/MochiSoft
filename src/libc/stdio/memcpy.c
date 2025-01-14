/* Gennaio 2025 - Davide Morazzo*/

#include "stdio.h"

void * memcpy (void *dest, void *source, size_t num){

	char *__dest = dest;
	char *__source = source;
	for(int i=0; i<num; i++){
		__dest[i] = __source[i];
	}

	return dest;
}