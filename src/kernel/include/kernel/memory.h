/* Davide Morazzo - March 2025 

Paging and kernel memory management:
Links:
- https://wiki.osdev.org/Paging

*/

#ifndef INCLUDE_KERNEL_MEMORY2_H
#define INCLUDE_KERNEL_MEMORY2_H

#include <stdint.h>
#include <stddef.h>

typedef struct PDE_s{
	uint32_t p:1;			// present
	uint32_t rw:1;			// 1:read+write, 0: read-only
	uint32_t us:1;			// 1: User, 0:supervisor
	uint32_t pwt:1;			// cache write-through enable
	uint32_t pcd:1;			// cache disable
	uint32_t a:1;			// accessed
	uint32_t avl2:1;
	uint32_t ps:1;
	uint32_t avl:4;		
	uint32_t addr:20;		// Address of page table
} PDE; // Page directory entry

typedef struct PDE_4MB_s{
	uint32_t p:1;			// present
	uint32_t rw:1;			// 1:read+write, 0: read-only
	uint32_t us:1;			// 1: User, 0:supervisor
	uint32_t pwt:1;			// cache write-through enable
	uint32_t pcd:1;			// cache disable
	uint32_t a:1;			// accessed
	uint32_t avl2:1;
	uint32_t ps:1;
	uint32_t avl:4;		
	uint32_t pat:1;			// Page attribute table
	uint32_t addrl:8;		// Address of page table
	uint32_t rsv:1;			// reserved = 0
	uint32_t addrh:10;		// Address of page table
} PDE_4MB; // Page directory entry 4MB page table

typedef struct PTE_s{
	uint32_t p:1;			// present
	uint32_t rw:1;			// 1:read+write, 0: read-only
	uint32_t us:1;			// 1: User, 0:supervisor
	uint32_t pwt:1;			// cache write-through enable
	uint32_t pcd:1;			// cache disable
	uint32_t a:1;			// accessed
	uint32_t d:1;			// dirty
	uint32_t pat:1;			// page attribute table
	uint32_t g:1;			// global		
	uint32_t avl:3;
	uint32_t addr:20;		// Address of page table
} PTE; // Page table entry

typedef void* phys_addr_t;
typedef void* virt_addr_t;

int memory_map(PDE *pd_base, void *phys_addr, void* virt_addr, size_t size);
phys_addr_t physical_addr(PDE *pd_base, virt_addr_t vaddr);

#endif