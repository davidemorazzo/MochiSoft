/* The bootloader will look at this image and start execution at the symbol
   designated as the entry point. */
ENTRY(_start)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* It used to be universally recommended to use 1M as a start offset,
	   as it was effectively guaranteed to be available under BIOS systems.
	   However, UEFI has made things more complicated, and experimental data
	   strongly suggests that 2M is a safer place to load. In 2016, a new
	   feature was introduced to the multiboot2 spec to inform bootloaders
	   that a kernel can be loaded anywhere within a range of addresses and
	   will be able to relocate itself to run from such a loader-selected
	   address, in order to give the loader freedom in selecting a span of
	   memory which is verified to be available by the firmware, in order to
	   work around this issue. This does not use that feature, so 2M was
	   chosen as a safer option than the traditional 1M. */
	. = 0x00100000;
	_kernel_start = . ;

	/* First put the multiboot header, as it is required to be put very early
	   in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */

	.multiboot.data : {
		*(.multiboot.data)
	}
	.multiboot.text : {
		*(.multiboot.text)
	}

	. += 0xC0000000;	
	.text ALIGN(4K) : AT(ADDR(.text) - 0xC0000000)
	{
		*(.text)
	}

	/* Read-only data. */
	.rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0000000)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}
	
	/* Definire l'heap_bottom (dopo i dati) */
    /* .heap : ALIGN(16) {
        PROVIDE(__heap_bottom = .);
		PROVIDE(__heap_top = . + 0x20000000);
        *(.heap)
    } */

	/* Definire lo stack_top (prima dell'inizio della memoria usata) */
    /* .stack : ALIGN(16)
	{
        PROVIDE(__stack_top = __heap_top + 0x20000000);  // Ad esempio, indirizzo alto della memoria 
		*(.stack)
    } */

	/* __heap_size = __heap_top - __heap_bottom;
	__stack_size = __stack_top - __heap_top; */

	_kernel_end = . ; 

	/* The compiler may produce other sections, by default it will put them in
	   a segment with the same name. Simply add stuff here as needed. */
}