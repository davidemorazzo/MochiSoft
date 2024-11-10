/* Novembre 2024 - Davide Morazzo

Variabili globali per i kernel di Mochi OS */
#include "time.h"

/* Contiene il tempo attuale letto dal RTC ad update finito.
Viene scritta solamente da IRQ8. Si aggiorna ogni secondo. */
time_t SYS_TIME_RTC;