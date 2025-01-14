/* Dicembre 2024 - Davide Morazzo */

#ifndef INCLUDE_DEV_BUS_INTERFACE_H
#define INCLUDE_DEV_BUS_INTERFACE_H

#include <stdint.h>

/* Interface abstracting the mass storage device driver. Drivers supported in OS:
- PCI/AHCI */
typedef struct storage_dev{
	int total_sectors;		// Total n. of addressable sectors
	int sector_size;		// Size in bytes of 1 sector
	int (*write)(uint32_t startl, uint32_t starth, uint32_t count, void * buf);
	int (*read)(uint32_t startl, uint32_t starth, uint32_t count, void * buf);
} storage_dev_t;

#endif