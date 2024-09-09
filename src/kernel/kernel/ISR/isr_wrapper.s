//  August 2024 - Davide Morazzo
//  Wrapper per gestire correttamente la ISR ed evitare che il
//  compilatore corrompa le funzioni usando solo inline assemble
//  Source: https://wiki.osdev.org/Interrupt_Service_Routines
// 
// #include "kernel/ISR/isr_wrapper.h"

// .globl isr_0x24_wrapper
// .align 4
// 
// isr_0x24_wrapper:
// 	pushal
// 	cld 					// ?
// 	call my_func 	// C function dove viene eseguito il codice
// 	popal
// 	iret					// Return from an interrupt call					
