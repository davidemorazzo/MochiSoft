//  August 2024 - Davide Morazzo
//  Wrapper per gestire correttamente la ISR ed evitare che il
//  compilatore corrompa le funzioni usando solo inline assemble
//  Source: https://wiki.osdev.org/Interrupt_Service_Routines


// EXCEPTIONS
.globl isr_0x00_wrapper
.globl isr_0x01_wrapper
.globl isr_0x02_wrapper
.globl isr_0x03_wrapper
.globl isr_0x04_wrapper
.globl isr_0x05_wrapper
.globl isr_0x06_wrapper
.globl isr_0x07_wrapper
.globl isr_0x08_wrapper
.globl isr_0x09_wrapper
.globl isr_0x0A_wrapper
.globl isr_0x0B_wrapper
.globl isr_0x0C_wrapper
.globl isr_0x0D_wrapper
.globl isr_0x0E_wrapper
.globl isr_0x0F_wrapper
.globl isr_0x10_wrapper
.globl isr_0x11_wrapper
.globl isr_0x12_wrapper
.globl isr_0x13_wrapper
.globl isr_0x14_wrapper
.globl isr_0x15_wrapper
.globl isr_0x16_wrapper
.globl isr_0x17_wrapper
.globl isr_0x18_wrapper
.globl isr_0x19_wrapper
.globl isr_0x1A_wrapper
.globl isr_0x1B_wrapper
.globl isr_0x1C_wrapper
.globl isr_0x1D_wrapper
.globl isr_0x1E_wrapper
.globl isr_0x1F_wrapper


// USER DEFINED
.align 4

isr_0x00_wrapper:
	pushal
	cld 					// ?
	call isr00 	// C function dove viene eseguito il codice
	popal
	iret					// Return from an interrupt call					

isr_0x01_wrapper:
	pushal
	cld 					// ?
	call isr01 	// C function dove viene eseguito il codice
	popal
	iret					// Return from an interrupt call		

isr_0x02_wrapper: pushal; cld; call isr02; popal; iret;
isr_0x03_wrapper: pushal; cld; call isr03; popal; iret;
isr_0x04_wrapper: pushal; cld; call isr04; popal; iret;
isr_0x05_wrapper: pushal; cld; call isr05; popal; iret;
isr_0x06_wrapper: pushal; cld; call isr06; popal; iret;
isr_0x07_wrapper: pushal; cld; call isr07; popal; iret;
isr_0x08_wrapper: pushal; cld; call isr08; popal; iret;
isr_0x09_wrapper: pushal; cld; call isr09; popal; iret;
isr_0x0A_wrapper: pushal; cld; call isr0A; popal; iret;
isr_0x0B_wrapper: pushal; cld; call isr0B; popal; iret;
isr_0x0C_wrapper: pushal; cld; call isr0C; popal; iret;
isr_0x0D_wrapper: pushal; cld; call isr0D; popal; iret;
isr_0x0E_wrapper: pushal; cld; call isr0E; popal; iret;
isr_0x0F_wrapper: pushal; cld; call isr0F; popal; iret;
isr_0x10_wrapper: pushal; cld; call isr10; popal; iret;
isr_0x11_wrapper: pushal; cld; call isr11; popal; iret;
isr_0x12_wrapper: pushal; cld; call isr12; popal; iret;
isr_0x13_wrapper: pushal; cld; call isr13; popal; iret;
isr_0x14_wrapper: pushal; cld; call isr14; popal; iret;
isr_0x15_wrapper: pushal; cld; call isr15; popal; iret;
isr_0x16_wrapper: pushal; cld; call isr16; popal; iret;
isr_0x17_wrapper: pushal; cld; call isr17; popal; iret;
isr_0x18_wrapper: pushal; cld; call isr18; popal; iret;
isr_0x19_wrapper: pushal; cld; call isr19; popal; iret;
isr_0x1A_wrapper: pushal; cld; call isr1A; popal; iret;
isr_0x1B_wrapper: pushal; cld; call isr1B; popal; iret;
isr_0x1C_wrapper: pushal; cld; call isr1C; popal; iret;
isr_0x1D_wrapper: pushal; cld; call isr1D; popal; iret;
isr_0x1E_wrapper: pushal; cld; call isr1E; popal; iret;
isr_0x1F_wrapper: pushal; cld; call isr1F; popal; iret;


