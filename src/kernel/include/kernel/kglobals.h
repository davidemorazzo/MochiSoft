/* Novembre 2024 - Davide Morazzo

Variabili globali per i kernel di Mochi OS */
#ifndef INCLUDE_KERNEL_KGLOBALS_H
#define INCLUDE_KERNEL_KGLOBALS_H 1

#include "time.h"
#include "kernel/modules/tty.h"

/* Contiene il tempo attuale letto dal RTC ad update finito.
Viene scritta solamente da IRQ8. Si aggiorna ogni secondo. */
extern time_t SYS_TIME_RTC;


/* Buffer circolare per immagazzinare l'input da uart0*/
extern struct circular_buf_rx TTY_CIRC_BUF_RX;

#endif