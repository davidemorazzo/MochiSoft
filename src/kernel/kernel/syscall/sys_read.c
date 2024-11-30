/* Novembre 2024 - Davide Morazzo */

#include "kernel/syscall.h"
#include "kernel/kglobals.h"
#include "stdio.h"

size_t sys_read(int fd, void *buf, size_t count){
	char *buf_ptr = (char*)buf;
	size_t res = 0;

	if (fd == stdin){
		for (size_t i=0; i<count; i++)			
			{
			if (TTY_CIRC_BUF_RX.read_ptr == TTY_CIRC_BUF_RX.write_ptr){
				break;
			}

			if (TTY_CIRC_BUF_RX.read_ptr >= (TTY_CIRC_BUF_RX.buf + TTY_CIRC_BUF_RX.size)){
				TTY_CIRC_BUF_RX.read_ptr = TTY_CIRC_BUF_RX.buf;
			}else{
				TTY_CIRC_BUF_RX.read_ptr++;
			}
			*buf_ptr = *TTY_CIRC_BUF_RX.read_ptr;
			buf_ptr++;
			res++;
		}
	}

	return res;
}