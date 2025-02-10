#include "dev/PCI/AHCI.h"
#include "kernel/kheap.h"
#include "kernel/kstdio.h"
#include "kernel/kglobals.h"

#include "string.h"

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
	MEMSET(PxCLB->cmdHeader, 0, 8);
	PxCLB->cmdHeader[0].cfl = sizeof(FIS_REG_H2D);
	PxCLB->cmdHeader[0].a = 0;
	PxCLB->cmdHeader[0].prdtl = 1;
	PxCLB->cmdHeader[0].p = 1;
	PxCLB->cmdHeader[0].c = 1;
	PxCLB->cmdHeader[0].ctba = (uint32_t) CmdTbl;
	
	/* H2D FIS with Identify command*/
	MEMSET(CmdTbl, 0, sizeof(HBA_CMD_TBL));
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


int lock_free_CLB_slot(AHCI_HDD_t * dev){
	HBA_PORT *port = dev->port;
	if (port == NULL) return -1;
	if ((void*)port->clb == NULL) return -1;
	
	uint32_t ci = port->ci;
	uint32_t sact = port->sact;

	for(int i=0; i<32; i++){
		// Check lock
		uint32_t eflags_reg = 0;
		__asm__(
			"bts %1, %2;"
			"pushf;"
			"pop %0;"
			: "=m" (eflags_reg)
			: "r" (i), "m" (dev->issued_cmd_mask));

		// Check CF to see if the bit was 0 before BTS
		if (!(eflags_reg & 1)){
			if (!(ci & 0x1) && !(sact & 0x1)){
				return i;
			}
		}		
		ci >>= 1;
		sact >>= 1;
	}

	return -1;
}


void issue_command(HBA_PORT *port, uint8_t cmdIndex){
	// KLOGERROR("%s: Command issued index %d", __func__, cmdIndex);
	port->ci |= 1 << cmdIndex;
}

static uint32_t check_pending_cmd(HBA_PORT *port){
	// uint32_t issued_cmd_mask = AHCI_HDD.issued_cmd_mask;
	AHCI_HDD.issued_cmd_mask &= port->ci;
}

void AHCI_interrupt_routine(void) {
	// KLOGINFO("AHCI_interrupt_routine()");
	// Service interrupt for port in use
	if (AHCI_HDD.abar->is & (0x1 << AHCI_HDD.portIndex)){
		HBA_PORT *port = AHCI_HDD.port;
		uint32_t P1IS = port->is;
		// KLOGINFO("\tP1IS=0x%X", P1IS);
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
				else if (it == 23){KLOGERROR("AHCI.PORT%d: Incorrect Port Multiplier Status", AHCI_HDD.portIndex)}
				else if (it == 24){KLOGERROR("AHCI.PORT%d: Overflow Status", AHCI_HDD.portIndex)}
				else if (it == 26){KLOGERROR("AHCI.PORT%d: Interface Non-fatal Error Status", AHCI_HDD.portIndex)}
				else if (it == 27){KLOGERROR("AHCI.PORT%d: Interface Fatal Error Status", AHCI_HDD.portIndex)}
				else if (it == 28){KLOGERROR("AHCI.PORT%d: Host Bus Data Error Status", AHCI_HDD.portIndex)}
				else if (it == 29){KLOGERROR("AHCI.PORT%d: Host Bus Fatal Error Status", AHCI_HDD.portIndex)}
				else if (it == 30){KLOGERROR("AHCI.PORT%d: Task File Error Status ", AHCI_HDD.portIndex)}
				else if (it == 31){KLOGERROR("AHCI.PORT%d: Cold Port Detect Status", AHCI_HDD.portIndex)}
			}
		}

		// Check pending commands
		check_pending_cmd(AHCI_HDD.port);
		// Interrupt Acknowledge
		port->is &= port->is;
	}

	return;
}

int AHCI_read_prim_dev (uint32_t startl, uint32_t starth, uint32_t count, void * buf){
	// find slot and "lock" the command slot
	int slot;
	uint32_t count_cpy  = count;
	do{
		slot = lock_free_CLB_slot(&AHCI_HDD);
	}while (slot == -1);

	HBA_CMD_HEADER * cmd_header = ((HBA_CMD_HEADER *)AHCI_HDD.port->clb) + slot;
	if (cmd_header==NULL) return -1;
	HBA_CMD_TBL * cmd_tbl = (HBA_CMD_TBL *) cmd_header->ctba;
	if (cmd_tbl == NULL) return -1;
	MEMSET(cmd_header, 0, 8); // Not clear CTBA, CTBAU
	MEMSET(cmd_tbl, 0, sizeof(HBA_CMD_TBL)); 
	cmd_header->cfl = sizeof(FIS_REG_H2D) / sizeof(uint32_t);
	cmd_header->prdtl = (uint16_t)((count-1)>>4) + 1; // ?

	/*  PRDT  */
	for(int i=0; i<cmd_header->prdtl-1; i++){
		cmd_tbl->prdt_entry[i].dba = ((uint32_t) buf) + i*16*AHCI_HDD.ident_packet.sector_bytes;
		cmd_tbl->prdt_entry[i].dbc = 16*AHCI_HDD.ident_packet.sector_bytes -1;
		cmd_tbl->prdt_entry[i].i = 1;
		count_cpy -= 16;
	}	
	// Last PRDT entry
	cmd_tbl->prdt_entry[cmd_header->prdtl-1].dba = ((uint32_t) buf) + 16*AHCI_HDD.ident_packet.sector_bytes*(cmd_header->prdtl-1);
	cmd_tbl->prdt_entry[cmd_header->prdtl-1].dbc = (count_cpy<<9)-1;
	cmd_tbl->prdt_entry[cmd_header->prdtl-1].i = 1;

	/* Setup Command */
	FIS_REG_H2D * fis = (FIS_REG_H2D *)cmd_tbl->cfis;
	if (fis == NULL) return -1;

	fis->fis_type = FIS_TYPE_REG_H2D;
	fis->c = 1;
	fis->command = 0xC8; // ATA_CMD_READ_DMA_EX
	fis->device = 1 << 6;
	fis->lba0 = (uint8_t) startl;
	fis->lba1 = (uint8_t) (startl >> 8);
	fis->lba2 = (uint8_t) (startl >> 16);
	fis->lba3 = (uint8_t) (startl >> 24);
	fis->lba4 = (uint8_t) (starth);
	fis->lba5 = (uint8_t) (starth >> 8);
	fis->countl = (uint8_t) count & 0xFF;
	fis->counth = (uint8_t) (count >> 8) & 0xFF;

	// Issue command and wait for completion
	issue_command(AHCI_HDD.port, slot);
	while((AHCI_HDD.port->ci >> slot) & 1);

	return cmd_header->prdbc;
}

int AHCI_write_prim_dev (uint32_t startl, uint32_t starth, uint32_t count, void *buf){
	// find slot and "lock" the command slot
	int slot;
	uint32_t count_cpy  = count;
	do{
		slot = lock_free_CLB_slot(&AHCI_HDD);
	}while (slot == -1);
	
	HBA_CMD_HEADER * cmd_header = ((HBA_CMD_HEADER *)AHCI_HDD.port->clb) + slot;
	if (cmd_header==NULL) return -1;

	HBA_CMD_TBL * cmd_tbl = (HBA_CMD_TBL *) cmd_header->ctba;
	if (cmd_tbl == NULL) return -1;

	MEMSET(cmd_header, 0, 8); // Not clear CTBA, CTBAU
	MEMSET(cmd_tbl, 0, sizeof(HBA_CMD_TBL)); 
	cmd_header->cfl = sizeof(FIS_REG_H2D) / sizeof(uint32_t);
	cmd_header->prdtl = (uint16_t)((count_cpy-1)>>4) + 1; // ?
	cmd_header->w = 1;

	/*  PRDT  */
	for(int i=0; i<cmd_header->prdtl-1; i++){
		cmd_tbl->prdt_entry[i].dba = ((uint32_t) buf) + i*16*AHCI_HDD.ident_packet.sector_bytes;
		cmd_tbl->prdt_entry[i].dbc = 16*AHCI_HDD.ident_packet.sector_bytes -1;
		cmd_tbl->prdt_entry[i].i = 1;
		count_cpy -= 16;
	}		
	// Last PRDT entry
	cmd_tbl->prdt_entry[cmd_header->prdtl-1].dba = ((uint32_t) buf) + 16*AHCI_HDD.ident_packet.sector_bytes*(cmd_header->prdtl-1);
	cmd_tbl->prdt_entry[cmd_header->prdtl-1].dbc = (count_cpy<<9)-1;
	cmd_tbl->prdt_entry[cmd_header->prdtl-1].i = 1;

	/* Setup Command */
	FIS_REG_H2D * fis = (FIS_REG_H2D *)cmd_tbl->cfis;
	if (fis == NULL) return -1;

	fis->fis_type = FIS_TYPE_REG_H2D;
	fis->c = 1;
	fis->command = 0xCA; // ATA_CMD_READ_DMA_EX
	fis->device = 1 << 6;
	fis->lba0 = (uint8_t) startl;
	fis->lba1 = (uint8_t) (startl >> 8);
	fis->lba2 = (uint8_t) (startl >> 16);
	fis->lba3 = (uint8_t) (startl >> 24);
	fis->lba4 = (uint8_t) (starth);
	fis->lba5 = (uint8_t) (starth >> 8);
	fis->countl = (uint8_t) count & 0xFF;
	fis->counth = (uint8_t) (count >> 8) & 0xFF;

	// Issue command and wait for completion
	issue_command(AHCI_HDD.port, slot);
	while((AHCI_HDD.port->ci >> slot) & 1);

	return cmd_header->prdbc;;
}

static void ATA_decode_str(char * source, char * dest, size_t size){
	for (unsigned int i=0; i<size-1; i+=2){
		dest[i] = source[i+1];
		dest[i+1] = source[i];
	}
	// Se size dispari
	if ((size % 2) != 0){
		dest[size-1] = source[size-1];
	}
}

void AHCI_init(AHCI_HDD_t * dev){
	HBA_MEM *abar = (HBA_MEM *) PCIgetHDDBAR5();
	if (abar == NULL){
		KLOGERROR("No AHCI device found on PCI");
		return;
	}
	
	uint8_t pi = abar->pi;
	dev->abar = abar;
	dev->port = NULL;
	for (int i=0; i<32; i++){
		if (pi & 1){
			uint32_t ssts = abar->ports[i].ssts; 
			uint8_t ipm = (ssts >> 8) & 0xF;
			uint8_t det = ssts & 0xF;
			if (ipm == 1 && det == 3){
				
				dev->portIndex = i;
				dev->port = &abar->ports[i];
				dev->port->clb = (uint32_t) &dev->cmd_list;
				dev->port->fb = (uint32_t) &dev->rcv_fis;
				for (int cmd=0; cmd <32; cmd++){
					dev->cmd_list.cmdHeader[cmd].ctba = (uint32_t) &dev->cmd_table[cmd];
				}
				dev->port->ie |= 0xFF;
				dev->abar->ghc |= 0x2; // Interrupt enable
				stop_cmd(dev->port);
				start_cmd(dev->port); 
				send_identify_cmd(dev->port, &AHCI_HDD.ident_packet);
				int capacity = dev->ident_packet.total_usr_sectors[0] * dev->ident_packet.sector_bytes;
				char model[41] = {'\0'};
				char tmp[41] = {'\0'};
				ATA_decode_str((char *)dev->ident_packet.model, tmp, 40);
				strcpy(rtrim(tmp), model);
				strcpy(model, dev->ident_packet.model);
				KLOGINFO("Storage on port %d: %s, capacity %d B, %d Bytes/sector", 
					i, model, capacity, dev->ident_packet.sector_bytes);
				return;
			}
		}
		pi >>= 1;
	}
	if (dev->port == NULL) {
		KLOGERROR("No active ports found on AHCI conf. space")
	}
}

storage_dev_t * AHCI_get_driver(storage_dev_t * driver){
	driver->read = &AHCI_read_prim_dev;
	driver->write = &AHCI_write_prim_dev;
	driver->sector_size = AHCI_HDD.ident_packet.sector_bytes;
	driver->total_sectors = AHCI_HDD.ident_packet.total_usr_sectors[0];
	return driver;
}