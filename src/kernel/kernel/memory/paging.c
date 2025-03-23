#include "kernel/memory.h"
#include "kernel/kheap.h"
#include "kernel/microcode.h"
#include "kernel/kstdio.h"

PTE *find_free_page(PDE *pd_base, PTE* pool, int pool_size){
	for (int i=0; i<pool_size; i++){
		uint32_t paging_addr = (uint32_t) physical_addr(NULL, &pool[i*1024]) >> 12; 
		char used = 0;
		for (int p=0; p<1024; p++){
			if (pd_base[p].addr == paging_addr){
				used = 1;
			}
		}
		if (used == 0){
			return &pool[i*1024];
		}
	}
	return NULL;
}

PTE *get_page(PDE *ptr_PDE, PTE* pool, int pool_size){
	phys_addr_t paddr_pt = (phys_addr_t)(ptr_PDE->addr << 12);
	PTE *res = NULL;
	for (int i=0; i<pool_size; i++){
		if (paddr_pt == physical_addr(NULL, &pool[i*1024])){
			res = &pool[i*1024];
			break;
		}
	}

	return res;
}

int memory_map(PDE *pd_base, void *phys_addr, void *virt_addr, size_t size){
	unsigned int pd_idx = (unsigned int) virt_addr >> 22;
	unsigned int pt_idx = (unsigned int) virt_addr >> 12 & 0x03FF;

	if (pd_base == NULL){
		pd_base = (PDE*)((uint32_t)get_page_dir() + 0xC0000000);
	}

	/* Allocazione del pool di pagine per il kernel nella heap */
	if (pt_pool == NULL){
		pt_pool = kmalloc_align(sizeof(PTE)*1024*50, 4096);
		if (pt_pool == NULL){
			return -1;
		}
	}

	while (size > 0){
		PDE *ptr_PDE = &pd_base[pd_idx];
		PTE *pt_base = NULL;
		PTE *ptr_PTE = NULL;

		if (ptr_PDE->addr == 0){
			pt_base = find_free_page(pd_base, pt_pool, 50);
			for (int i=0; i<1024; i++){
				((uint32_t*) pt_base)[i] = 0;
			}
			ptr_PDE->addr = (uint32_t)physical_addr(NULL, pt_base) >> 12;
		} else {
			pt_base = get_page(ptr_PDE, pt_pool, 50);
		}
		
		if (pt_base == NULL){
			return -1;
		}
		ptr_PTE = &pt_base[pt_idx];
		ptr_PDE->p = 1;
		ptr_PDE->rw = 1;

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

	__asm__ volatile (
		"mov %cr3, %eax;"
    	"mov %eax, %cr3;"
	);

}

phys_addr_t physical_addr(PDE *pd_base, virt_addr_t vaddr){

	if (pd_base == NULL){
		pd_base = get_page_dir();
	}
	unsigned int pt_entry = ((uint32_t)vaddr >> 12) & 0x3FF;
	unsigned int pd_entry = (uint32_t)vaddr >> 22;
	if(pd_base[pd_entry].addr == 0){
		KLOGERROR("%s: pd_base[pd_entry].addr == 0", __func__);
		return NULL;
	}
	PTE *ptr_PTE = (PTE*)(pd_base[pd_entry].addr<<12);
	if(ptr_PTE[pt_entry].addr == 0){
		KLOGERROR("%s: ptr_PTE[pt_entry].addr == 0", __func__);
		return NULL;
	}
	void *page_base = (void*)(ptr_PTE[pt_entry].addr<<12);
	return (phys_addr_t)((uint32_t)page_base | ((uint32_t)vaddr & 0xFFF));
}

phys_addr_t get_page_dir(){
	return (phys_addr_t) cr3();
}