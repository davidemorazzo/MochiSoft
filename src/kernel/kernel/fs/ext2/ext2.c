#include "kernel/fs/ext2/ext2.h"
#include <stddef.h>
#include "kernel/kheap.h"
#include "kernel/kstdio.h"
#include "string.h"


int ext2_read_inode_blocks(storage_dev_t *driver, Ext2_inode_t *inode, void *buf){
	void * bufptr = buf;
	int count = 0;
	for (int i=0; i<12; i++){
		if (inode->ptr_blk[i] != 0){
			ext2_read_blocks(driver, bufptr, inode->ptr_blk[i], 1);
			bufptr = (void*)(((char *)buf)+1024);
			count += 1024;
		}
	}

	return count;
	// TODO: support indirect blocks
}

Ext2_inode_t ext2_inode_from_path(storage_dev_t *driver, Ext2_inode_t *cur_dir, char * path){
	
	Ext2_inode_t inode = {0};
	Ext2_inode_t i_root = {0};
	if(!path || !driver || !cur_dir){
		KLOGERROR("%s: NULL argument", __func__);
		return inode;}

	char last_call = 0;
	int dirname_len = -1;
	char dirname[EXT2_FNAME_MAX_LEN+1] = {'\0'};
	Ext2_directory_t *dir_entry;
	Ext2_superblock_t sblk = ext2_get_sblk(driver);

	/* Get subdirectory name */
	dirname_len=strfind('/', path, strlen(path));
	if(dirname_len == -1){
		/* No '/' trovato quindi in `path` c'è il filename.
		non ci sono più cartelle da trovare. */
		strcpy(path, dirname);
		last_call = 1;
	}else if(dirname_len == 0){
		/* '/' in posizione zero, quindi path assoluto. cur_dir=root*/
		i_root = ext2_get_inode(driver, 2);
		cur_dir = &i_root;
		dirname_len=strfind('/', path+1, strlen(path+1)) + 1;
		strcpyn(path+1, dirname, dirname_len - 1);
	}else{
		strcpyn(path, dirname, dirname_len);
	}

	size_t dir_size = ((cur_dir->sizel / (1024 << sblk.block_size_log2)) + 1) * 1024;
	void * dir_buf = kmalloc (dir_size);
	if (!dir_buf){return inode;}
	
	/* Read current directory content and find matching subdirectory */
	ext2_read_inode_blocks(driver, cur_dir, dir_buf);
	dir_entry = (Ext2_directory_t *) dir_buf;

	while(dir_entry->inode != 0){
		char subdir_name[EXT2_FNAME_MAX_LEN+1] = {'\0'};
		strcpyn(&dir_entry->name, subdir_name, dir_entry->name_len);
		if (strcmp(subdir_name, dirname) == 0){
			/* Sottocartella trovata, chiamata ricorsiva */
			Ext2_inode_t next_inode = ext2_get_inode(driver, dir_entry->inode);
			kfree(dir_buf);
			if (last_call){
				/* Fine ricorsione */
				return next_inode;
			}else{
				/* Chiamata ricorsiva su sottocartella */
				return ext2_inode_from_path(driver, &next_inode, &path[dirname_len+1]);
			}
		}
		dir_entry = (Ext2_directory_t *) (((char*)dir_entry)+dir_entry->size);
	}
	KLOGERROR("%s: '%s' not found", __func__, dirname);
	kfree(dir_buf);
	return inode;
}

Ext2_superblock_t ext2_get_sblk(storage_dev_t *driver){
	char buf[1024];
	Ext2_superblock_t sblk;
	if (driver != NULL){
		driver->read(EXT2_BLK0_SECTOR+2,0,2,buf);
	}
	memcpy(&sblk, buf, sizeof(Ext2_superblock_t));
	return sblk;
}

/* Translate the block number to the sector LBA*/
uint64_t ext2_get_block_LBA( storage_dev_t *driver, uint64_t blk_idx){
	Ext2_superblock_t sblk = ext2_get_sblk(driver);
	int64_t blk_size = 1024 << sblk.block_size_log2;
	int blk_sector_size = (int) blk_size / driver->sector_size;
	return EXT2_BLK0_SECTOR + blk_idx * blk_sector_size;
}

int ext2_present(storage_dev_t * driver){
	if (driver == NULL) return 0;
	if (driver->read == NULL) return 0;
	if (driver->write == NULL) return 0;
	if (driver->sector_size < 4) return 0;

	Ext2_superblock_t sblk = ext2_get_sblk(driver);
	return (sblk.signature == EXT2_SIGNATURE);
}

Ext2_blk_grp_desc_t ext2_get_blk_desc_tbl(storage_dev_t *driver, uint64_t blk_grp_idx){
	static char buffer[2048];
	ext2_read_blocks(driver, buffer, 2, 2);	
	Ext2_blk_grp_desc_t bgd;
	memcpy(&bgd, &((Ext2_blk_grp_desc_t*)buffer)[blk_grp_idx], sizeof(Ext2_blk_grp_desc_t));
	return bgd;
}

Ext2_inode_t ext2_get_inode(storage_dev_t * driver, uint32_t idx){
	// uint8_t buffer[4096];
	Ext2_inode_t inode = {0};
	if (!driver){
		KLOGERROR("%s: NULL argument", __func__);
		return inode;
	}
	Ext2_superblock_t sblk = ext2_get_sblk(driver);
	if (idx >= sblk.inodes_total){
		KLOGERROR("%s: Block index out of range", __func__);
	}

	/* Calculate block group index */
	uint32_t blk_size = (1024 << sblk.block_size_log2);
	uint32_t blk_grp_addr = (idx-1) / sblk.block_groups_inodes;
	Ext2_blk_grp_desc_t bgd = ext2_get_blk_desc_tbl(driver, blk_grp_addr);
	
	int local_inode_idx = (idx % sblk.block_groups_inodes) -1;
	static char buffer[4096]; // TODO fare fetch del blocco giusto e non tutta la tabella inode
	ext2_read_blocks(driver, buffer, bgd.start_blk_iaddr, 4);
	int inode_size = 128;
	if (sblk.version_major >= 1){
		inode_size = sblk.inode_size;
	}
	memcpy(&inode, ((char*)buffer)+local_inode_idx*inode_size, sizeof(Ext2_inode_t));
	return inode;
}


void * ext2_read_blocks(storage_dev_t *driver, void *buffer, uint32_t block_idx, size_t count){
	if (!driver || !buffer){
		KLOGERROR("%s: NULL argument", __func__);
		return NULL;
	}

	Ext2_superblock_t sblk = ext2_get_sblk(driver);
	if (block_idx >= sblk.blocks_total){
		KLOGERROR("%s: Block index out of range", __func__);
	}
	uint64_t lba = ext2_get_block_LBA (driver, block_idx);
	int block_size = (1024<<sblk.block_size_log2); 
	int sector_cnt = block_size*count / driver->sector_size;
	uint32_t cnt = driver->read(lba & 0xFFFFFFFF, (lba >> 32) & 0xFFFFFFFF,
			sector_cnt, buffer);
	if (cnt != (block_size*count)){
		KLOGERROR("ext2_read_blocks: read error");
		return NULL;
	}

	return buffer;
}