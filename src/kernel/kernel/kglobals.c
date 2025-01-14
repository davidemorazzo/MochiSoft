/* Davide Morazzo - Novembre 2024 

Inizializzazione delle variabili globali */

#include "kernel/kglobals.h"
#include "kernel/kheap.h"
#include "dev/PCI/AHCI.h"

time_t SYS_TIME_RTC = 0; 

struct circular_buf_rx TTY_CIRC_BUF_RX = {
	NULL,
	NULL,
	NULL,
	200,
};

struct AHCI_HDD_s AHCI_HDD = {
	NULL,	// TODO rimappare? 
	NULL,
	{0},
	{0},
	{0},
	0
};
