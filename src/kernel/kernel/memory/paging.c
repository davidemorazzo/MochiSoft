#include "kernel/memory.h"
#include "kernel/kheap.h"

int memory_map(PDE *pd_base, void *phys_addr, void *virt_addr, size_t size){
	unsigned int pd_idx = ((unsigned int)virt_addr & 0xFFFFF000) / 0x400000;
	unsigned int pt_idx = (((unsigned int)virt_addr & 0xFFFFF000) / 4096) % 1024;

	while (size > 0){
		PDE *ptr_PDE = &pd_base[pd_idx];
		if (ptr_PDE->addr == 0){
			*(uint32_t*)ptr_PDE = (size_t)kmalloc_align(1024*sizeof(PTE), 4096) + 3;
			for (int i=0; i< 1024; i++){
				((uint32_t*)(ptr_PDE->addr<<12))[i] = 0; 
			}
		}
		PTE *ptr_PTE = &((PTE *)(ptr_PDE->addr<<12))[pt_idx];
		ptr_PTE->addr = ((uint32_t)phys_addr & 0xFFFFF000)>>12;
		ptr_PTE->p = 1;
		ptr_PTE->rw = 1;

		phys_addr += 4096;
		size -= 4096;

		if (pt_idx >= 1023){
			pt_idx = 0;
			pd_idx ++;
		}else{
			pt_idx++;
		}
	}

	__asm__(
		"mov %%cr3, %%ecx;"
    	"mov %%ecx, %%cr3;" : :);

}

phys_addr_t physical_addr(PDE *pd_base, virt_addr_t vaddr){
	unsigned int pt_entry = ((uint32_t)vaddr >> 12) & 0x3FF;
	unsigned int pd_entry = (uint32_t)vaddr >> 22;
	PTE *ptr_PTE = (PTE*)(pd_base[pd_entry].addr<<12);
	void *page_base = (void*)(ptr_PTE[pt_entry].addr<<12);
	return (phys_addr_t)((uint32_t)page_base | ((uint32_t)vaddr & 0xFFF));
}