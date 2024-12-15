/* Dicembre 2024 - Davide Morazzo 
Funzioni chiamate dalla routine _start che è eseguita prima
di `kernel_main`. */

#include <stdint.h>

void idpaging(uint32_t *first_pte, uint32_t from, int size) {
    from = from & 0xfffff000; // discard bits we don't want
    for(; size>0; from += 4096, size -= 4096, first_pte++){
       *first_pte = from | 1;     // mark page present.
    }
 }