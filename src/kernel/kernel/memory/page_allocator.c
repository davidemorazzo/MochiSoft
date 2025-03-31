#include "kernel/memory.h"

uint8_t *page_alloc_bitmap = NULL;

void phys_page_alloc(size_t n_pages, phys_addr_t *pages){
	size_t pg_cnt=0;
	for (size_t i=0; i<PAGE_ALLOC_BITMAP_SZ || pg_cnt<=n_pages; i++){
		if (page_alloc_bitmap[i] != 0xFF){
			// slot di 8 pagine trovato
			for (unsigned short j=0; j<8; j++){
				if ((page_alloc_bitmap[i]>>j)&0x1){
					// Pagina libera trovata
					page_alloc_bitmap[i] |= 1<<j;
					unsigned int bit_idx = i*8 + j;
					pages[pg_cnt] = (phys_addr_t)(bit_idx * PAGE_ALLOC_PAGE_SZ);
					pg_cnt++;
				}
			}
		}
	}
	return;
}

void phys_page_dealloc(size_t n_pages, phys_addr_t *pages){
	for (unsigned short i=0; i<n_pages; i++){
		unsigned int bit_idx = (unsigned int)pages[n_pages] / PAGE_ALLOC_PAGE_SZ;
		unsigned short offset = bit_idx % 8;
		page_alloc_bitmap[bit_idx / 8] &= (1<<offset);
	}
	return;
}

void page_alloc_init(phys_addr_t base){
    page_alloc_bitmap = (uint8_t*) base;
    for (unsigned int i=0; i<PAGE_ALLOC_BITMAP_SZ; i++){
        page_alloc_bitmap[i] = 0;
    }

	for (unsigned int i=0; i<(0x800000/PAGE_ALLOC_PAGE_SZ/8); i++){
        page_alloc_bitmap[i] = 0xFF;
    }
}