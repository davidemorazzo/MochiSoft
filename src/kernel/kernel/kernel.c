#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "string.h"

#include "kernel/gdt.h"
#include "kernel/idt.h"
#include "kernel/microcode.h"
#include "kernel/modules/tty.h"
#include "kernel/kstdio.h"
#include "kernel/exceptions.h"
#include "kernel/kheap.h"
#include "kernel/syscall.h"
#include "kernel/kglobals.h"
#include "dev/PCI/AHCI.h"
#include "kernel/fs/ext2/ext2.h"
#include "kernel/memory.h"

#include "time.h"
#include "stdio.h"
#include "string.h"

#define KERNEL_STACK_SIZE 0x4000 // 16kb

uint64_t global_IDT[255] = {0};
uint64_t global_GDT[50] = {0};

/* Funzione di entry point del kernel, richiamata con lo stesso nome
nel bootloader (src/bootloader/boot.s)*/


void generic_isr(){
    __asm__("pushal");
    __asm__("popal; leave; iret"); /* BLACK MAGIC! */
}

void kernel_main (void){
    // extern unsigned int __stack_top;
    // extern unsigned int __stack_size;
    // extern unsigned int __heap_bottom;
    // extern unsigned int __heap_top;
    // extern unsigned int __heap_size;
    // unsigned int *sp = &__stack_top;
    // unsigned int *ss = &__stack_size;
    // unsigned int *hb = &__heap_bottom;
    // unsigned int *ht = &__heap_top;
    // unsigned int *hs = &__heap_size;
    
    disable_it();   

    /* ======= GLOBAL DESCRIPTOR TABLE ====== */
    gdt_init(global_GDT);
    xDTR GDTR;
    GDTR.base=(uint64_t*)physical_addr(NULL, global_GDT);
    GDTR.length=3*8-1;
    gdt_load(GDTR);

    /* ===== SETUP HEAP ====== */

    extern unsigned int _kernel_end;
    phys_addr_t heap_start = (phys_addr_t) ((unsigned int)&_kernel_end + PAGE_ALLOC_BITMAP_SZ); 
    _kAllocStatus* kHeapStatus = _setupHeap(heap_start, 0xC07FFFFF-(size_t)(&_kernel_end));
    // // char * array0, *array1;
    // array0 = kmalloc(5*sizeof(char));
    // array1 = kmalloc(5*sizeof(char));
    // array0[0] = 0xFF;
    // array1[0] = 0xFF;
    // array0[1] = 0xFF;
    // array1[1] = 0xFF;
    // array0[4] = 0xFF;
    // array1[4] = 0xFF;
    // _kfree(array0, 5*sizeof(char));
    // _kfree(array1, 5*sizeof(char));


    /* ============ PAGING SETUP ============ */
    page_alloc_init(&_kernel_end);


    /* ========== SETUP STACK ================ */
    memory_map(NULL, 
        (phys_addr_t)(0x40000000-KERNEL_STACK_SIZE), 
        (virt_addr_t)(0xFFC00000-KERNEL_STACK_SIZE), 
        KERNEL_STACK_SIZE);
    __asm__("mov $0xFFBFFFFF, %esp");

    

    /* ===== INTERRUPT DESCRIPTOR TABLE ====== */
    xDTR IDTR;
    IDTR.base = (uint64_t*)physical_addr(NULL,global_IDT);
    IDTR.length = 256*8-1;
    load_idt(IDTR); /*Load IDTR*/

    InterruptDescriptor32 genericIsrDesc;
    genericIsrDesc.zero=0x0;
    genericIsrDesc.type_attributes = 0x8E;
    genericIsrDesc.offset_1 = ((uint32_t) generic_isr) & 0xFFFF;
	genericIsrDesc.offset_2 = (((uint32_t) generic_isr) >> 16) & 0xFFFF;
    genericIsrDesc.selector = 0x8; /*RPL=0;TI=0;segment_index=1*/

    for (int k=0; k<256; k++){
        global_IDT[k] = (*(uint64_t*) &genericIsrDesc);
    }

    /*Setup logger del kernel*/
    serial_init(UART0);

    setup_exc_it();

    extern void init_devs();
    init_devs(); 
    KLOGINFO("Devices inizializzati")
    enable_it();    /*Interrupt Enable Flag = 1. (EFLAGS register)*/


    if (gdt_check(GDTR)){
        KLOGINFO("GDTR ok, base address: 0x%X", GDTR.base);
    }else{
        KLOGERROR("GDTR content not consistent!");
    }

    if (check_idt(IDTR)){
        KLOGINFO("IDTR ok, base address: 0x%X", IDTR.base);
    }else{
        KLOGERROR("IDTR content not consistent!");
    }


    /*Boot Welcome text*/
    time_t now;
    do{
        sys_time(&now);
    }while(now == 0);
    kprint("MochiSoft Inc. (R) 2024\n\nWelcome in MochiSoft OS!\n");
    kprint("%s\n\n", asctime(gmtime(&now)));
    KLOGINFO("Avvio MochiOS completato");

    // storage_dev_t driver;
    // AHCI_get_driver(&driver);
    // char buf[10000];
    // driver.read(EXT2_BLK0_SECTOR, 0, 10, buf);
    // if (ext2_present(&driver)){
    //     KLOGINFO( "Ext2 filesystem detected");
        
    //     Ext2_inode_t root = ext2_get_inode(&driver, 2);
    //     Ext2_inode_t inode = ext2_inode_from_path(&driver, &root, "/dir1/decentralized-estimation-and-control-for-multisensor-systems.pdf");
    //     char *file_buf = (char *) kmalloc(inode.sizel);
    //     ext2_read_inode_blocks(&driver, &inode, file_buf);
    //     KLOGINFO("cat %s:\n%s", "dir1/dir2/test.file", file_buf);
    //     // ext2_read_inode_blocks(&driver, &inode, file_buf);        
    //     /* Ext2_superblock_t sblk = ext2_get_sblk(&driver);
    //     Ext2_blk_grp_desc_t bgd = ext2_get_blk_desc_tbl(&driver, 0);
    //     char *buffer[1024];
    //     ext2_read_blocks(&driver, buffer, bgd.start_blk_iaddr, 1);
    //     Ext2_inode_t i_root = ((Ext2_inode_t*)buffer)[2];
    //     ext2_read_blocks(&driver, buffer, i_root.ptr_blk[0], 1);
    //     // Ext2_inode_t i = ext2_get_inode(&driver, 12);
    //     // ext2_read_blocks(&driver, buffer, i.ptr_blk[13], 1);
    //     // ext2_read_blocks(&driver, buffer, *(uint32_t*)buffer[0], 1);

    //     KLOGINFO("List \"/\":");
    //     Ext2_directory_t *d_root = (void *)buffer;
    //     while (d_root->inode != 0){
    //         Ext2_inode_t i = ext2_get_inode(&driver, d_root->inode);
    //         char name[200] = {0};
    //         KLOGINFO("Inode: %d, %s", d_root->inode, memcpy(name, &d_root->name, d_root->name_len));
    //         if (i.type_permissions & 0x8000){
    //             char contenuto[1024] = {'\0'};
    //             ext2_read_blocks(&driver, contenuto, i.ptr_blk[0], 1);
    //             KLOGINFO("\t%s", trim(contenuto));
    //         }
    //         d_root = (void *)(((char*)d_root)+d_root->size);
    //     } 
    //     */
    // }

    PID_t p = sys_create_process("proc0", "/home/proc0");
    
    extern void sched_isr();
    SET_IT_VEC(genericIsrDesc, sched_isr, 100);
    asm("mov $1, %eax;"
        "mov $2, %ecx;"
        "mov $3, %edx;"
        "mov $4, %ebx;"
        "mov $5, %ebp;"
        "mov $6, %esi;"
        "mov $7, %edi;");
    asm("int $100");

    while(1){

    }
    // Kernel function is exiting here
}