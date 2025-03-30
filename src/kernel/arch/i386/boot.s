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
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 #16 16KiB
stack_top:

/* Allocate tables for paging */
.global boot_page_directory
.global boot_page_table0
.global boot_page_table768
.global boot_page_table769
.section .bss, "aw", @nobits
	.align 4096
boot_page_directory:
	.skip 4096
boot_page_table0:
	.skip 4096
boot_page_table768:
	.skip 4096
boot_page_table769:
	.skip 4096

/* Entry point of the kernel in _start */
    // mov $__stack_top, %esp #initialization of the stack pointer to the top of the stack (grows downward)
    // /* TODO Altre cose fare per cose più avanzate, GDT, paging ... */
    // call kernel_main
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
# Identity map 1st MB = 256 pages in boot_page_table0
	movl $(boot_page_table0 - 0xC0000000), %edi # destination
	movl $0, %esi # starting address
	movl $1024, %ecx
1:
	movl %esi, %edx
	orl $0x3, %edx
	movl %edx, (%edi)
	addl $4096, %esi
	addl $4, %edi
	loop 1b

2:
# Map _kernel_start to 0xC0000000, size 8MB = 2048 pages
	movl $0x0, %esi # starting address
	movl $(boot_page_table768 - 0xC0000000), %edi # destination
	movl $2048, %ecx
3:
	movl %esi, %edx
	orl $0x3, %edx
	movl %edx, (%edi)
	addl $4096, %esi
	addl $4, %edi
	loop 3b

4:
	movl $(boot_page_table0 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 0
	movl $(boot_page_table768 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 768 * 4
	movl $(boot_page_table769 - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 769 * 4
	movl $(boot_page_directory - 0xC0000000 + 0x003), boot_page_directory - 0xC0000000 + 1023*4

	# Set cr3 to the address of the boot_page_directory.
	movl $(boot_page_directory - 0xC0000000), %ecx
	movl %ecx, %cr3

	# Enable paging and the write-protect bit.
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	# Jump to higher half with an absolute jump. 
	# lea 4f, %ecx
	lea kernel_main, %ecx
	jmp *%ecx

.section .text
4:
    movl $0, boot_page_directory + 0 // unmap identity mapping of kernel
    movl %cr3, %ecx
    movl %ecx, %cr3 // Reload CR3 to force TLB flush 
    // initialization of the stack pointer to the top of the stack (grows downward)
    mov $stack_top, %esp
    // mov $__stack_top, %esp
    
    /* ----------------------------- */
    /* |  MAIN KERNEL ENTRY-POINT  | */
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
