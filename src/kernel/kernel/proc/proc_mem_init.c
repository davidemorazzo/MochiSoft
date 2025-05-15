#include "kernel/memory.h"
#include "kernel/kheap.h"

void proc_mem_init (PDE *pd, pages_range_t *phys_pages){
	pd = (PDE*)kmalloc(sizeof(PDE)*1024);
	/* Allocate code (kernel) */
	extern unsigned int _kernel_start;
	extern unsigned int _kernel_end;
	size_t kernel_size = (size_t) &_kernel_end - (size_t)&_kernel_start;
	kernel_size &= 0xFFFFF000;
	kernel_size += 8192;		//4K aligned size
	memory_map(pd, (phys_addr_t)&_kernel_start, 0xC0000000, kernel_size);
	
	/* Allocate heap */

	/* Allocate process stack */

	/* Self mapping */
}