/* Novembre 2024 - Davide Morazzo

Variabili globali per i kernel di Mochi OS */
#ifndef INCLUDE_KERNEL_KGLOBALS_H
#define INCLUDE_KERNEL_KGLOBALS_H 1

#include "time.h"
#include "kernel/modules/tty.h"
#include "dev/PCI/AHCI.h"

/* Contiene il tempo attuale letto dal RTC ad update finito.
Viene scritta solamente da IRQ8. Si aggiorna ogni secondo. */
extern time_t SYS_TIME_RTC;


/* Buffer circolare per immagazzinare l'input da uart0*/
extern struct circular_buf_rx TTY_CIRC_BUF_RX;

struct AHCI_HDD_s {
	HBA_MEM *abar;			/* AHCI base register the HDD in use */
	HBA_PORT *port;			/* AHCI port port for the HDD in use */
	HBA_CMD_LIST cmd_list; 	/* Command list structure, linked into port->clb. Allocated in heap*/
	HBA_CMD_TBL __attribute__((aligned(128))) cmd_table[32]; /* Command tables, linked into port->clb->ctba */
	HBA_FIS rcv_fis;		/* Received FIS, linked into port->fb. Allocated in heap */
	uint32_t portIndex;		/* Port index of the HDD in use */
};

/* AHCI hard disk controller in use */
extern struct AHCI_HDD_s AHCI_HDD;

#endif