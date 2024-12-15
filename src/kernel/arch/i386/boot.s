/*Constants for the multiboot header*/
.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

/* Multiboot header */
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

// Initial stack
// .section .bootstrap_stack, "aw", @nobits
// stack_bottom:
// .skip 16384 #16 16KiB
// stack_top:

/* Allocate tables for paging */
.section .bss
    .align 4096
boot_page_directory:
    .skip 4096
kernel_page_table:
    .skip 4096
identity_page_table:
    .skip 4096

/* Entry point of the kernel in _start */
.section .multiboot.text, "a"
.global _start
// .type _start, @function
// _start:
    // mov $__stack_top, %esp #initialization of the stack pointer to the top of the stack (grows downward)
    // /* TODO Altre cose fare per cose più avanzate, GDT, paging ... */
    // call kernel_main
// .type _start
_start: 
    // Disable paging
    movl $0, %ecx
    movl %ecx, %cr0
    // Phisical address of boot page 1
    mov $(stack_top-0xC0000000), %esp
    push $0x100000                          // size
    push $0                                 // from
    push $(identity_page_table - 0xC0000000)// first_pte
    call idpaging
    add $12, %esp

//     mov $(identity_page_table - 0xC0000000), %eax   // page table entry ptr
//     mov $0x100, %ecx
//     mov $0, %edx                                    // address ptr
// 10:
//     or $0x1, %edx
//     mov %edx, (%eax)
//     add $4096, %edx
//     add $4, %eax
//     loop 10b

    // %edi -> destination index reg.
    // %esi -> source index reg.
    // %ecx -> count register
    movl $(kernel_page_table - 0xC0000000), %edi // boot_page_talbe1 phy address -> EDI
    movl $0, %esi                               // first address to map (0x00)  -> ESI 
    movl $1023, %ecx                            // Load count reg. to iterate 1023 times
    
1:
    cmpl $_kernel_start, %esi
    jl 2f                   // if _kernel_start > ESI                   then GOTO 2
    cmpl $(_kernel_end - 0xC0000000), %esi
    jge 3f                  // if physical_addr(_kernel_end) <= ESI     then GOTO 3
    // Map physical address as "present, writeable"
    // TODO: mark .text, .rodata as non writeable.
    movl %esi, %edx
    orl $0x003, %edx
    movl %edx, (%edi)
2:
    addl $4096, %esi    // size of page is 4096
    addl $4, %edi       // Size of entries in page table is 4 bytes
    loop 1b             // ECX--; GOTO 1b if ECX!=0
3:
    // The two page directories used are at index 0 and at index 768
    // Map page table to both virtual addresses 0x00000000 and 0xC0000000
    movl $(kernel_page_table - 0xC0000000 + 0x3), boot_page_directory - 0xC0000000 + 0
    movl $(kernel_page_table - 0xC0000000 + 0x3), boot_page_directory - 0xC0000000 + (768 * 4)
    // Set CR3 to the physical address of the boot_page_directory
    movl $(boot_page_directory - 0xC0000000), %ecx
    movl %ecx, %cr3
    // Enable paging and write-protect bits
    movl %cr0, %ecx
    orl $0x80000001, %ecx
    movl %ecx, %cr0
    // Jump to higher half kernel with absolute jump
    lea 4f, %ecx
    jmp *%ecx

.section .text
4:
    movl $0, boot_page_directory + 0 // unmap identity mapping of kernel
    movl %cr3, %ecx
    movl %ecx, %cr3 // Reload CR3 to force TLB flush 
    // initialization of the stack pointer to the top of the stack (grows downward)
    // mov $stack_top, %esp
    mov $__stack_top, %esp
    
    /*  ----------------------------- */
    /* |  MAIN KERNEL ENTRY-POINT   | */
    /* ----------------------------- */
    call kernel_main
    
    /* Nothing more to do. Setup infinite loop */
    cli #disable interrupts
1:  hlt
    jmp 1b

/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/

// .size _start, . - _start
