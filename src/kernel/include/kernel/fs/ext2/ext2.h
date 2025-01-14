/* Dicembre 2024 - Davide Morazzo */

#ifndef INCLUDE_KERNEL_FS_EXT2_EXT2_H
#define INCLUDE_KERNEL_FS_EXT2_EXT2_H
#include "kernel/fs/ext2/defs.h"
#include "dev/bus_interface.h"
#include <stddef.h>

#define EXT2_SIGNATURE 0xEF53
#define EXT2_BLK0_SECTOR 2048

/* Checks Ext2 superblock exists on the device */
int ext2_present(storage_dev_t * driver);

/* Get filesystem superblock */
Ext2_superblock_t ext2_get_sblk(storage_dev_t *driver);

/* Get descriptor table element 0*/
Ext2_blk_grp_desc_t ext2_get_blk_desc_tbl(storage_dev_t *driver, uint64_t blk_grp_idx);

/* Get inode structure from inode index. Inode indexes start from 1*/
Ext2_inode_t ext2_get_inode(storage_dev_t * driver, uint32_t idx);

/* Read blocks */
void * ext2_read_blocks(storage_dev_t *driver, void *buffer, uint32_t block_idx, size_t count);

#endif