#include "kernel/fs/fs.h"
#include "kernel/fs/fs_internal.h"
#include "kernel/fs/ext2/ext2.h"
#include "kernel/kstdio.h"
#include "kernel/kheap.h"
#include "kernel/microcode.h"
#include "dev/PCI/AHCI.h"


file_desc fs_open_file(char * path){
	if (!path){
		KLOGERROR("%s: NULL argument", __func__);
	}

	unsigned int inode_id = ext2_inode_from_path(&fs_state.driver, 2, path);
	Ext2_inode_t inode = ext2_get_inode(&fs_state.driver, inode_id);
	file_desc null_f_desc = {0};

	/* Lock array */
	int success = 0;
	for (int c=0; c<100000; c++){
		if (bit_test_set(&fs_state.array_lock, 0)){
			success = 1;
			break;
		}
	}
	if (success == 0){
		KLOGERROR("%s: fs_state.array_lock timeout", __func__);
		return null_f_desc;
	}

	/* Get lock on the file */
	// Controllare se file è già aperto
	for (unsigned int i=0; i<fs_state.file_handler_len; i++){
		if (fs_state.file_handler_array[i] != NULL &&
			fs_state.file_handler_array[i]->id.source && 
			inode_id == fs_state.file_handler_array[i]->id.id && 
			fs_state.file_handler_array[i]->open){
				/* File already opened*/
				KLOGWARN("%s: file already open", __func__);
				fs_state.array_lock = 0;
				return null_f_desc;
		}
	}

	struct fs_file_handler_k * file_handler = NULL;

	// Trovare uno slot per l'handler o allocare un nuovo slot alla file
	for (unsigned int i=0; i<fs_state.file_handler_len; i++){
		if (fs_state.file_handler_array[i] == NULL){

			fs_state.file_handler_array[i] = (struct fs_file_handler_k*) kmalloc (sizeof(struct fs_file_handler_k));
			file_handler = fs_state.file_handler_array[i];
			if (file_handler == NULL){
				fs_state.array_lock = 0;
				return null_f_desc;
			}
			file_desc out_f_desc = {0};
			out_f_desc.id = inode_id;
			out_f_desc.source = 0;
			file_handler->id = out_f_desc;
			file_handler->open = 1;
			fs_state.array_lock = 0;
			file_handler->buffer_size = inode.sizel + fs_state.driver.sector_size;
			file_handler->buffer = kmalloc(file_handler->buffer_size);
			file_handler->read_ptr = 0;
			file_handler->file = inode;
			ext2_read_inode_blocks(&fs_state.driver, &file_handler->file, file_handler->buffer);
			break;
		}
	}

	if (file_handler == NULL){
		KLOGERROR("%s: max number of files already open", __func__);
		fs_state.array_lock = 0;
		return null_f_desc;
	}

	return file_handler->id;
}
