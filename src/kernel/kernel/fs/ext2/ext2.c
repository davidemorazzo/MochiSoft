#include "kernel/fs/ext2/ext2.h"
#include <stddef.h>
#include "kernel/kheap.h"
#include "kernel/kstdio.h"

Ext2_superblock_t ext2_get_sblk(storage_dev_t *driver){
	char buf[1024];
	Ext2_superblock_t sblk;
	if (driver != NULL){
		driver->read(EXT2_BLK0_SECTOR+2,0,2,buf);
	}
	// TODO: Poor man memcpy
	for (int i=0; i<1024; i++){
		char * ptr = (char *) &sblk;
		ptr[i] = buf[i];
	}
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
	uint64_t blk_idx;
	Ext2_superblock_t sblk = ext2_get_sblk(driver);
	
	int blk_size = 1024 << sblk.block_size_log2;
	blk_idx = blk_grp_idx * sblk.block_groups_blocks + 2;
	uint64_t bgdt_lba = ext2_get_block_LBA(driver, blk_idx);
	int bgdt_len = (sblk.blocks_total / sblk.block_groups_blocks) + 1;
	int bgdt_size = sizeof(Ext2_blk_grp_desc_t)*bgdt_len;
	void *blk_tbl = (void *)kmalloc(bgdt_size + 512);
	int b = driver->read(bgdt_lba & 0xFFFFFFFF, (bgdt_lba>>32) & 0xFFFFFFFF, 
		bgdt_size/512 + 1, blk_tbl);
	Ext2_blk_grp_desc_t bgd;
	memcpy(&bgd, &((Ext2_blk_grp_desc_t*)blk_tbl)[blk_grp_idx], sizeof(Ext2_blk_grp_desc_t));
	return bgd;
}

Ext2_inode_t ext2_get_inode(storage_dev_t * driver, uint32_t idx){
	// uint8_t buffer[4096];
	Ext2_inode_t inode = {0};
	Ext2_superblock_t sblk = ext2_get_sblk(driver);

	/* Calculate block group index */
	uint32_t blk_size = (1024 << sblk.block_size_log2);
	uint32_t blk_grp_addr = (idx-1) / sblk.block_groups_inodes;
	// Ext2_blk_grp_desc_t bgdt = ext2_get_blk_desc_tbl(driver, blk_grp_addr);

	/* Calculate offset for inode table */
	uint32_t inode_tbl_offset = 3; // including super-block, data block bitmap, inode bitmap
	inode_tbl_offset += sblk.blocks_total / sblk.block_groups_blocks +1;

	/* Get inode table of this block group */
	uint32_t inode_size = 128;
	if (sblk.version_major >= 1) {}; // TODO: implement check of inode size from SBLK
	uint32_t tbl_size_bytes  = sblk.block_groups_inodes * inode_size;
	uint32_t tbl_size_sectors = (tbl_size_bytes / driver->sector_size)+1;
	char * buffer = (char *)kmalloc(tbl_size_sectors * driver->sector_size);
	Ext2_inode_t *i;
	if (buffer != NULL){
		uint64_t lba = ext2_get_block_LBA(driver, blk_grp_addr+inode_tbl_offset);
		uint32_t cnt = driver->read(lba & 0xFFFFFFFF, (lba >> 32) & 0xFFFFFFFF,
			6, buffer);
		i = (Ext2_inode_t *)buffer;
	} 
	
		// if (cnt != blk_size){
		// 	KLOGERROR("Read failure!");
		// 	return inode;
		// }
	uint32_t block = (blk_grp_addr * inode_size) / blk_size;
	// TODO: substitue with memcpy
	for (uint32_t i=0; i<blk_size; i++){
		char * ptr = (char *) &inode;
		ptr[i] = buffer[i]; 
	}
	return inode;
}


void * ext2_read_blocks(storage_dev_t *driver, void *buffer, uint32_t block_idx, size_t count){
	Ext2_superblock_t sblk = ext2_get_sblk(driver);
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