/*August 2024 - Davide Morazzo */

#ifndef INCLUDE_KERNEL_INTERRUPT_H
#define INCLUDE_KERNEL_INTERRUPT_H 1


void general_exception_handler() {
    __asm__("pushal");
    /* do something */
	volatile int x=0;
	x+=10;
    __asm__("popal; leave; iret"); /* BLACK MAGIC! */
}

#endif