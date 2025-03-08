#include "kernel/memory.h"

int memory_map(PDE *pd_base, void *phys_addr, void *virt_addr, size_t size){
	// unsigned int pd_idx = ((unsigned int)virt_addr & 0xFFFFF000) / 0x400000;
	// unsigned int pt_idx = ((unsigned int)virt_addr & 0xFFFFF000) % 0x400000;

	// while (size > 0){
	// 	PDE *ptr_PDE = &pd_base[pd_idx];
	// 	PTE *ptr_PTE = &((PTE *)ptr_PDE->addr)[pt_idx];
	// 	ptr_PTE->addr = (uint32_t)phys_addr & 0xFFFFF000;
	// 	ptr_PDE->p = 1;
	// 	ptr_PDE->rw = 1;

	// 	phys_addr += 4096;
	// 	size -= 4096;

	// 	if (pt_idx >= 1023){
	// 		pt_idx = 0;
	// 		pd_idx ++;
	// 	}else{
	// 		pt_idx++;
	// 	}
	// }

}