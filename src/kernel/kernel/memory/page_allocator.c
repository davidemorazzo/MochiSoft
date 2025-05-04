#include "kernel/memory.h"
#include "kernel/kstdio.h"
#include "kernel/microcode.h"

uint8_t *page_alloc_bitmap = NULL;

/* Allocatore di pagine fisiche. Le pagine sono tracciate con la bitmap
page_alloc_bitmap. Metodo usato è slab-allocator. 
Arguments:
	- size_t n_pages:		Numero di pagine da allocare

Return:
	- phys_addr_t:			base della slab allocata. Se NULL allocazione fallita 
*/
phys_addr_t phys_page_alloc(size_t n_pages){
	unsigned int offset = 0;
	for (size_t base=0; base<(PAGE_ALLOC_BITMAP_SZ-n_pages); base++){
		size_t bit_idx = base + offset;
		if ((page_alloc_bitmap[(bit_idx)/8] >> (bit_idx%8)) == 0){
			offset++;
			if (offset >= n_pages){
				for (size_t i=base; i<(base+offset); i++){
					phys_page_reserve((phys_addr_t)(i*PAGE_ALLOC_PAGE_SZ));
				}
				return (phys_addr_t)(base*PAGE_ALLOC_PAGE_SZ);
			}
		}else{
			offset = 0;
		}
	}
	KLOGERROR("%s: Couldn't allocate %d consecutive pages", __func__, n_pages);
	return NULL;
}

void phys_page_dealloc(size_t n_pages, phys_addr_t *pages){
	for (unsigned short i=0; i<n_pages; i++){
		unsigned int bit_idx = (unsigned int)pages[n_pages] / PAGE_ALLOC_PAGE_SZ;
		unsigned short offset = bit_idx % 8;
		page_alloc_bitmap[bit_idx / 8] &= (1<<offset); // FIXME: ??
	}
	return;
}

void page_alloc_init(phys_addr_t base){
    page_alloc_bitmap = (uint8_t*) base;
    for (unsigned int i=0; i<PAGE_ALLOC_BITMAP_SZ; i++){
        page_alloc_bitmap[i] = 0;
    }

	// Riservare le pagine mappate a kernel, heap iniziale ...
	for (unsigned int i=0; i<(0x800000/PAGE_ALLOC_PAGE_SZ/8); i++){
        page_alloc_bitmap[i] = 0xFF;
    }
}

/* Funzione per marcare certe pagine di memoria come già riservate. Da usare per
esempio per i dispositivi memory mapped ed evitare che vengano usati come memoria normale. 
Argomenti:
	- phys_addr_t paddr:		Indirizzo allineato 4Kb che indica la pagina in memoria 

Ritorno:
	- int:
		0: Pagina già riservata
		1: Pagina riservata
*/
int phys_page_reserve(phys_addr_t paddr){
	unsigned int bit_idx = (unsigned int)paddr / PAGE_ALLOC_PAGE_SZ;
	unsigned short offset = bit_idx % 8;
	short state = page_alloc_bitmap[bit_idx / 8] >> offset;
	page_alloc_bitmap[bit_idx / 8] |= (1<<offset);

	return (int)state == 0;
	 
}