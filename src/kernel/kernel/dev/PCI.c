#include "dev/PCI/PCI.h"
#include "kernel/microcode.h"
#include "kernel/kstdio.h"

uint32_t PCIconfigReadDWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset){
	uint32_t address;
	uint32_t busl = (uint32_t)bus;
	uint32_t slotl = (uint32_t)slot;
	uint32_t funcl = (uint32_t)func;

	// Select configuration address
	address = (uint32_t)((busl << 16) | (slotl << 11) |
              (funcl << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
	outl(PCI_ADDR, address);
	// Read in the data
	return inl(PCI_DATA);
}

uint16_t PCIconfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset){
	uint32_t address;
	uint32_t busl = (uint32_t)bus;
	uint32_t slotl = (uint32_t)slot;
	uint32_t funcl = (uint32_t)func;
	uint16_t tmp = 0;

	// Select configuration address
	address = (uint32_t)((busl << 16) | (slotl << 11) |
              (funcl << 8) | (offset & 0xFC) | ((uint32_t)0x80000000));
	outl(PCI_ADDR, address);
	// Read in the data
	tmp = (uint16_t)((inl(PCI_DATA) >> ((offset & 2) * 8)) & 0xFFFF);
	return tmp;
}

void PCIcheckAllBuses(void){
	uint32_t bus;
	uint32_t device;
	for (bus = 0; bus < 256; bus++){
		for (device = 0; device < 32; device++){
			uint32_t vendorID = (uint32_t) PCIconfigReadWord(bus, device, 0, 0);
			if (vendorID != 0xFFFF) {
				uint32_t deviceID = (uint32_t)PCIconfigReadWord(bus, device, 0, 2);
				uint32_t class = (uint32_t) PCIconfigReadWord (bus, device, 0, 10);
				KLOGINFO ("PCI device detected: bus %d, dev %d, PCI id %X:%X", 
					bus, device, vendorID, deviceID);

				// Mass storage devices	
				if (class == 0x106){
					uint32_t progIF = (uint32_t) PCIconfigReadWord (bus, device, 0, 8) >> 8;
					if (progIF == 0x1){
						uint32_t bar5 = PCIconfigReadDWord(bus, device, 0, 0x24);
						KLOGINFO("\t^^^ AHCI controller, ABAR at 0x%X", bar5);
					}
				}
			}
		}
	}
}

// Returns first HDD with AHCI base register
void * PCIgetHDDBAR5(void){
	uint32_t bus;
	uint32_t device;
	for (bus = 0; bus < 256; bus++){
		for (device = 0; device < 32; device++){
			uint32_t vendorID = (uint32_t) PCIconfigReadWord(bus, device, 0, 0);
			if (vendorID != 0xFFFF) {
				uint32_t class = (uint32_t) PCIconfigReadWord (bus, device, 0, 10);
				// Mass storage devices	
				if (class == 0x106){
					// Return BAR5 = ABAR
					return (void *) PCIconfigReadDWord(bus, device, 0, 0x24);
				}
			}
		}
	}
	return NULL;
}