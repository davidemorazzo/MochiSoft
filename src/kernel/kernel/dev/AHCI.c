#include "dev/PCI/AHCI.h"
#include "kernel/kheap.h"
#include "kernel/kstdio.h"
#include "kernel/kglobals.h"

#define MEMSET(startptr, val, size) 				\
for (	char * __ptr = (char *)startptr; 			\
			__ptr < ( ((char *)startptr) + size ); 	\
			__ptr++){ 							\
		*__ptr = val; 							\
	}

void stop_cmd(HBA_PORT *port)
{
	// Clear ST (bit0)
	port->cmd &= ~HBA_PxCMD_ST;

	// Clear FRE (bit4)
	port->cmd &= ~HBA_PxCMD_FRE;

	// Wait until FR (bit14), CR (bit15) are cleared
	while(1)
	{
		if (port->cmd & HBA_PxCMD_FR)
			continue;
		if (port->cmd & HBA_PxCMD_CR)
			continue;
		break;
	}

}

void start_cmd(HBA_PORT *port)
{
	// Wait until CR (bit15) is cleared
	while (port->cmd & HBA_PxCMD_CR)
		;

	// Set FRE (bit4) and ST (bit0)
	port->cmd |= HBA_PxCMD_FRE;
	port->cmd |= HBA_PxCMD_ST; 
}


void send_identify_cmd(HBA_PORT *port, SATA_ident_t *buf){

	volatile HBA_CMD_LIST * PxCLB = (HBA_CMD_LIST *)port->clb; // 32 elements
	volatile HBA_CMD_TBL *CmdTbl = (HBA_CMD_TBL*) PxCLB->cmdHeader[0].ctba;

	// Command Header 0 setup
	uint8_t * ptr = (uint8_t *)PxCLB->cmdHeader;
	MEMSET(ptr, 0, sizeof(PxCLB->cmdHeader));
	PxCLB->cmdHeader[0].cfl = sizeof(FIS_REG_H2D);
	PxCLB->cmdHeader[0].a = 0;
	PxCLB->cmdHeader[0].prdtl = 1;
	PxCLB->cmdHeader[0].p = 1;
	PxCLB->cmdHeader[0].c = 1;
	PxCLB->cmdHeader[0].ctba = (uint32_t) CmdTbl;
	
	/* H2D FIS with Identify command*/
	MEMSET(CmdTbl, 0, sizeof(CmdTbl));
	FIS_REG_H2D *fis = (void *)&CmdTbl->cfis;
	MEMSET(fis, 0, sizeof(FIS_REG_H2D));
	fis->fis_type = FIS_TYPE_REG_H2D;
	fis->command = 0xEC; // ATA identify
	fis->c = 1;

	// PRDT entry
	CmdTbl->prdt_entry[0].dbc = 0x1FF; //((uint32_t)(1<<31)) | ((uint32_t)0x000001FF);
	CmdTbl->prdt_entry[0].dba = (uint32_t) buf;
	CmdTbl->prdt_entry[0].i = 0;

	// issue command
	AHCI_HDD.issued_cmd_mask |= 1;
	port->ci = 1;


	// // Aspettare completamento
	while (port->ci & 0x1);
	return;
}


int find_free_CLB_slot(HBA_PORT *port){
	if (port == NULL) return -1;
	if (port->clb == NULL) return -1;
	
	uint32_t ci = port->ci;
	uint32_t sact = port->sact;

	for(int i=0; i<32; i++){
		if ((ci & 0x1) && (sact & 0x1)){
			return i;
		}
		ci >>= 1;
		sact >>= 1;
	}

	return -1;
}


void issue_command(HBA_PORT *port, uint8_t cmdIndex){
	port->ci |= 1 << cmdIndex;
}

static uint32_t check_pending_cmd(HBA_PORT *port){
	uint32_t issued_cmd_mask = AHCI_HDD.issued_cmd_mask;
	AHCI_HDD.issued_cmd_mask &= port->ci;
}

void AHCI_interrupt_routine(void) {
	KLOGINFO("AHCI_interrupt_routine()");
	// Service interrupt for port in use
	if (AHCI_HDD.abar->is & (0x1 << AHCI_HDD.portIndex)){
		HBA_PORT *port = AHCI_HDD.port;
		uint32_t P1IS = port->is;
		for (int it=0; it<32; it++,P1IS>>=1){
			if (P1IS & 0x1){
				if (it==0){			/* Device to Host Register FIS Interrupt */
					// KLOGINFO("   Device to Host Register FIS Interrupt")
				}else if (it == 1){	/* PIO Setup FIS Interrupt */
					// KLOGINFO("   PIO Setup FIS Interrupt")
				}else if (it == 2){ /* DMA Setup FIS Interrupt */
					// KLOGINFO("   DMA Setup FIS Interrupt")
				}else if(it == 3){ 	/* Set Device Bits Interrupt */
					// KLOGINFO("   Set Device Bits Interrupt")
				}else if(it == 4){ 	/* Unknown FIS Interrupt */
					// KLOGINFO("   Unknown FIS Interrupt")
				}
			}
		}

		// Check pending commands
		check_pending_cmd(AHCI_HDD.port);
		// Interrupt Acknowledge
		port->is &= port->is;
	}

	return;
}