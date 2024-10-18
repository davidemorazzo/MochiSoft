/* Davide Morazzo - Settembre 2024 

Implementazione di chiamate di sistema per lo user space */
#include "time.h"

void isr_0x80_wrapper();

time_t time(time_t *tloc);

