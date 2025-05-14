/* Dicembre 2024 - Davide Morazzo */

#ifndef INCLUDE_KERNEL_FS_FS_H
#define INCLUDE_KERNEL_FS_FS_H

#include <stdint.h>


/* File descriptor che indica la struttura memorizzata nel kernel quando un file viene aperto 
	STDOUT = 1
	STDIN = 2
	STDERR = 3
	...
*/
typedef struct file_desc_s{
	/* Identificativo unico per la risorsa. L'identificativo può 
	essere duplicato attraverso più sorgenti di file */
	uint32_t id;
	/* Sorgente delle risorsa:
	- 0 : HDD filesystem
	- 1 : Kernel I/O
	*/
	uint8_t source; 
} file_desc;


// fs_create_file()

// int fs_read_file(char * path, void * buffer);

// fs_update_file()

// fs_delete_file()

 file_desc fs_open_file(char * path);

// fs_close_file()


#endif