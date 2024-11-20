/* Davide Morazzo - Novembre 2024 */

#include "kernel/syscall.h"
#include "stdio.h"
#include "kernel/uart.h"
#include "kernel/modules/tty.h"

// TODO: per ora supporta solo STDOUT come file descriptor
size_t sys_write(int fd, const void *buf, size_t count){
	size_t write_cnt = 0;
	if (buf == NULL){
		return -1;
	}

	switch(fd){
		case stdout:
			for (size_t i=0; i<count; i++){
				uart_write(UART0, ((char*)buf)[i]);
				write_cnt++;
			}
			break;
		
		case stderr:
			serial_writestring(UART0, "\x1b[91m");
			for (size_t i=0; i<count; i++){
				serial_putchar(UART0, ((char*)buf)[i]);
				write_cnt++;
			}
			serial_writestring(UART0, "\x1b[0m");
			break;
			
		default:
			write_cnt = -1;
			break;
	}

	return write_cnt;
}