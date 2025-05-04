#include "kernel/memory.h"
#include "kernel/kheap.h"
#include "kernel/microcode.h"
#include "kernel/kstdio.h"


/* Funzione per mappare memoria fisica alla memory space di un certo processo. La memoria
fisica specificata in paddr deve essere già allocata al processo con phys_page_alloc
contiguamente per il numero di pagine specificate. La funzione è da usare quando 
la page directory da modificare è caricata in CR3 
Argomenti:
- *pd_base:	Page directory. Non usato al momento
- paddr:	Indirizzo fisico da mappare. Deve già essere allocato al processo
- vaddr:	Indirizzo nello spazio di memoria del processo
- size: 	Numero di bytes consecutivi da mappare

Ritorno:
- int:	
	-1: Non riuscito
	0: Riuscito
*/
int memory_map(PDE *pd_base, phys_addr_t paddr, virt_addr_t vaddr, size_t size){
	// unsigned int pd_idx = (unsigned int) vaddr >> 22;
	// unsigned int pt_idx = (unsigned int) vaddr >> 12 & 0x03FF;
	paddr = (phys_addr_t) ((unsigned int) paddr & 0xFFFFF000);

	while (size > 0){
		// Funziona solo in user-space :(
		PDE *pde = (PDE*) (0xFFFFF000+((unsigned int)vaddr>>22)*4);
		if (pde->addr == 0){
			/* Allocare una nuova page table in memoria fisica */
			// TODO: Questo è un memory leak. Pagina da rilasciare alla chiusura del processo
			phys_addr_t page = phys_page_alloc(1);
			if (page== NULL){return -1;}
			
			pde->addr = (unsigned int)page >> 12;
			pde->p = 1;
			pde->rw = 1;
		}
		PTE *pte = (PTE*) (0xFFC00000+((unsigned int)vaddr>>12)*4);
		pte->p = 1;
		pte->rw = 1;
		pte->addr = (unsigned int)paddr >> 12;
		
		paddr += 4096;
		vaddr += 4096;
		size -= 4096;
	}

	__asm__ volatile (
		"mov %cr3, %eax;"
    	"mov %eax, %cr3;"
	);

	return 0;

}

phys_addr_t physical_addr(PDE *pd_base, virt_addr_t vaddr){

	if (pd_base == NULL){
		pd_base = get_page_dir();
	}
	// unsigned int pt_entry = ((uint32_t)vaddr >> 12) & 0x3FF;
	// unsigned int pd_entry = (uint32_t)vaddr >> 22;
	PDE *pd = (PDE*) (0xFFFFF000+((unsigned int)vaddr>>22)*4);
	PTE *pt = (PTE*) (0xFFC00000+((unsigned int)vaddr>>12)*4);
	
	if(pd == NULL || pd->p == 0){
		KLOGERROR("%s: PDE not present. Address not mapped", __func__);
		return NULL;
	}
	// PTE *ptr_PTE = (PTE*)(pd_base[pd_entry].addr<<12);
	if(pt == NULL || pd->p == 0){
		KLOGERROR("%s: PTE not present. Address not mapped", __func__);
		return NULL;
	}

	void *page_base = (void*)(pt->addr<<12);
	return (phys_addr_t)((uint32_t)page_base | ((uint32_t)vaddr & 0xFFF));
}

phys_addr_t get_page_dir(){
	return (phys_addr_t) cr3();
}