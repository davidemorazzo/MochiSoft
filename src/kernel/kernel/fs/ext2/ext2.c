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
	Ext2_superblock_t sblk = ext2_get_sblk(driver);

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