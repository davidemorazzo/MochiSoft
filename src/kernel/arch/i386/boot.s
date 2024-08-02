/*Constants for the multiboot header*/
.set ALIGN,     1<<0
.set MEMINFO,   1<<1
.set FLAGS, ALIGN | MEMINFO
.set MAGIC, 0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

/* Multiboot header */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Stack initialization and definition */
.section .bss
.align 16
stack_bottom:
.skip 16384  #16KiB stack size
stack_top:

/* Entry point of the kernel in _start */
.section .text
.global _start
.type _start, @function
_start:
    mov $stack_top, %esp #initialization of the stack pointer to the top of the stack (grows downward)
    /* TODO Altre cose fare per cose più avanzate, GDT, paging ... */
    call kernel_main

    /* Nothing more to do. Setup infinite loop */
    cli #disable interrupts
1:  hlt
    jmp 1b

/*
Set the size of the _start symbol to the current location '.' minus its start.
This is useful when debugging or when you implement call tracing.
*/

.size _start, . - _start
