#include "string.h"

void strcpyn(char * source, char * dest, size_t n){
    for(size_t i=0; i<n; i++){
        dest[i] = source[i];
    }
}

void strcpy(char * source, char * dest){
    size_t size = strlen(source);
    
    for(size_t i=0; i<size; i++){
        dest[i] = source[i];
    }
    dest[size] = '\0';
}