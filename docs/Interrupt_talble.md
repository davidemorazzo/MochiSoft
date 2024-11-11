# Interrupt table layout

La handler function è la funzione C, nella IDT viene inserito un wrapper scritto in ASM per gestire correttamente l'acknowledge dell'interrupt.



| Index | Class     | Handler func.     | Notes             |		
| ----- |------     |--------------     |------             |
| 0x00	| Exception | isr00				|					|
| 0x01	| Exception | 					| Reserved			|
| 0x02	| Exception | isr02				|					|
| 0x03	| Exception | 					| Reserved			| 
| 0x04	| Exception | isr04				|					|
| 0x05	| Exception | isr05				|					|
| 0x06	| Exception | isr06				|					|
| 0x07	| Exception | isr07				|					| 
| 0x08	| Exception | isr08				|					|
| 0x09	| Exception | 					| Reserved			|
| 0x0A	| Exception | isr0A				|					|
| 0x0B	| Exception | isr0B				|					| 
| 0x0C	| Exception | isr0C				|					|
| 0x0D	| Exception | isr0D				|					|
| 0x0E	| Exception | isr0E				|					|
| 0x0F	| Exception | 					| Reserved			| 
| 0x10	| Exception | isr10				|					|
| 0x11	| Exception | isr11				|					|
| 0x12	| Exception | isr12				|					|
| 0x13	| Exception | isr13				|					| 
| 0x14	| Exception | isr14				|					|
| 0x15	| Exception | isr15				|					|
| 0x16	| Exception | 					| Reserved			|
| 0x17	| Exception | 					| Reserved			| 
| 0x18	| Exception | 					| Reserved			|
| 0x19	| Exception | 					| Reserved			|
| 0x1A	| Exception | 					| Reserved			|
| 0x1B	| Exception | 					| Reserved			| 
| 0x1C	| Exception | isr1C				|					|
| 0x1D	| Exception | isr1D				|					|
| 0x1E	| Exception | isr1E				|					|
| 0x1F	| Exception | 					| Reserved			|
| 0X20	| PIC1 IRQ	| irq0				| PIT, unmasked		|
| 0X21	| PIC1 IRQ	| 					|					|
| 0X22	| PIC1 IRQ	| 					| PIC2 daisy-chain  |
| 0X23	| PIC1 IRQ	| 					|					|
| 0X24	| PIC1 IRQ	| irq4				| UART0, unmasked	|
| 0X25	| PIC1 IRQ	| 					|					|
| 0X26	| PIC1 IRQ	| 					|					|
| 0X27	| PIC1 IRQ	| 					|					|
| 0X28	| PIC2 IRQ	| irq8				| RTC, unmasked		|
| 0X29	| PIC2 IRQ	| 					|					|
| 0X2A	| PIC2 IRQ	| 					|					|
| 0X2B	| PIC2 IRQ	| 					|					|
| 0X2C	| PIC2 IRQ	| 					|					|
| 0X2D	| PIC2 IRQ	| 					|					|
| 0X2E	| PIC2 IRQ	| 					|					|
| 0X2F	| PIC2 IRQ	| 					|					|
| 0x30	| User def. | 					|					|
| ...	| 			|					|					|
| 0x80	| User def.	| syscall_handler	| System call		|
| ...	| 			|					|					|
