/* Febbraio 2025 - Davide Morazzo */

#ifndef INCLUDE_KERNEL_FS_FS_INTERNAL_H
#define INCLUDE_KERNEL_FS_FS_INTERNAL_H

#include <stdint.h>
#include "kernel/fs/fs.h"
#include "kernel/fs/ext2/ext2.h"

/* Struttura tenuta all'interno del kernel che tiene
   le informazioni relative ad un file aperto. E' 
   'richiamata' da un processo usando `file_desc`.*/
struct fs_file_handler_k{
	file_desc id;					// Unique identifier
	Ext2_inode_t file;				// Inode number (ext2)
	uint64_t read_ptr; 				// Number of bytes read from beg of file
	unsigned short int open;		// File is in use.
	void * buffer;					// Buffer in kernel heap. Contains buffer_size bytes
	unsigned int buffer_size;		// Size of `buffer`
};

/* Variabili globali che immagazzinano lo stato
   del file system. Allocata sulla heap del kernel */
struct fs_state_s{
	storage_dev_t driver;
	struct fs_file_handler_k * file_handler_array[128];
	uint8_t array_lock;					// Lock flag 
	unsigned int file_handler_len;
};

extern struct fs_state_s fs_state;

/* Inizializza file system all'avvio del kernel */
void init_fs();

#endif