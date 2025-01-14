/* Dicembre 2024 - Davide Morazzo */

#ifndef INCLUDE_KERNEL_FS_EXT2_EXT2_H
#define INCLUDE_KERNEL_FS_EXT2_EXT2_H
#include "kernel/fs/ext2/defs.h"
#include "dev/bus_interface.h"

#define EXT2_SIGNATURE 0xEF53
#define EXT2_BLK0_SECTOR 2050

/* Checks Ext2 superblock exists on the device */
int ext2_present(storage_dev_t * driver);

/* Get descriptor table element 0*/
Ext2_blk_grp_desc_t ext2_get_blk_desc_tbl(storage_dev_t * driver);

#endif