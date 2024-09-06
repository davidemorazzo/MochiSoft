#include "string.h"

void strcpy(char * source, char * dest, size_t n){
    for(size_t i=0; i<n; i++){
        dest[i] = source[i];
    }
}