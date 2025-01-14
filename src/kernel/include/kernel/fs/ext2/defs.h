/* Dicembre 2024 - Davide Morazzo

Definizioni per il driver del filesystem Ext2 */

#ifndef INCLUDE_KERNEL_FS_EXT2_DEFS_H
#define INCLUDE_KERNEL_FS_EXT2_DEFS_H

#include <stdint.h>

typedef enum ext2_fs_state{
	FILE_SYSTEM_CLEAN = 1,
	FILE_SYSTEM_HAS_ERRORS = 2,
} ext2_fs_state_t;

typedef enum ext2_error_handling{
	IGNORE_ERROR = 1,				//Ignore the error (continue on)
	REMOUNT_READ_ONLY = 2,			//Remount file system as read-only
	KERNEL_PANIC = 3,				//Kernel panic
} ext2_error_handling_t;

typedef struct Ext2_superblock{
	uint32_t inodes_total;			// Total number of inodes in file system
	uint32_t blocks_total;			// Total number of blocks in file system
	uint32_t blocks_superuser;		// Number of blocks reserved for superuser (see offset 80)
	uint32_t blocks_unallocated;	// Total number of unallocated blocks
	uint32_t inodes_unallocated;	// Total numver of unallocated inodes
	uint32_t superblock_block_addr; // Block number of the block containing the superblock (also the starting block number, NOT always zero.)
	uint32_t block_size_log2;		// log2 (block size) - 10. (In other words, the number to shift 1,024 to the left by to obtain the block size)
	uint32_t fragment_size_log2;	// log2 (fragment size) - 10. (In other words, the number to shift 1,024 to the left by to obtain the fragment size)
	uint32_t block_groups_blocks;	// Number of blocks in each block group
	uint32_t block_groups_fragments;// Number of fragments in each block group
	uint32_t block_groups_inodes;	// Number of inodes in each block group
	uint32_t last_mnt_time;			// Last mount time (in POSIX time)
	uint32_t last_wrt_time;			// Last written time (in POSIX time)
	uint16_t count_mnt;				// Number of times the volume has been mounted since its last consistency check (fsck)
	uint16_t count_mnt_allowed;		// Number of mounts allowed before a consistency check (fsck) must be done
	uint16_t signature;				// Ext2 signature (0xef53), used to help confirm the presence of Ext2 on a volume
	uint16_t fs_state;				// File system state. Refer to `ext2_fs_state_t`
	uint16_t error_action;			// What to do when an error is detected. Refer to `ext2_error_handling_t`
	uint16_t version_minor;			// 	Minor portion of version (combine with Major portion below to construct full version field)
	uint32_t fsck_time;				// POSIX time of last consistency check (fsck)
	uint32_t fsck_max_interval;		// Interval (in POSIX time) between forced consistency checks (fsck)
	uint32_t creation_os_id;		// Operating system ID from which the filesystem on this volume was created (see below)
	uint32_t version_major;			// Major portion of version (combine with Minor portion above to construct full version field)
	uint16_t reserved_blocks_uid;	// User ID that can use reserved blocks
	uint16_t reserved_blocks_gid;	// Group ID that can use reserved blocks
} Ext2_superblock_t;

typedef struct Ext2_blk_grp_desc{
	uint32_t bitmap_blk_addr;		// Block address of block usage bitmap
	uint32_t bitmap_inode_addr;		// Block address of inode usage bitmap
	uint32_t start_blk_iaddr;		// Starting block address of inode table
	uint16_t blk_unallocated;		// Number of unallocated blocks in group
	uint16_t inodes_unallocated;	// Number of unallocated inodes in group
	uint16_t dir_count;				// Number of directories in group
	uint8_t unused[14];
}Ext2_blk_grp_desc_t;

typedef struct Ext2_inode{
	uint16_t type_permissions;		// Type and Permissions (see below)
	uint16_t user_id;				// User ID
	uint32_t sizel;					// Lower 32 bits of size in bytes
	uint32_t time_last_access;		// Last Access Time (in POSIX time)
	uint32_t time_creation;			// Creation Time (in POSIX time)
	uint32_t time_last_modification;// Last Modification time (in POSIX time)
	uint32_t time_delete;			// Deletion time (in POSIX time)
	uint16_t group_id;				// Group ID
	uint16_t hard_links_count;		// Count of hard links (directory entries) to this inode. When this reaches 0, the data blocks are marked as unallocated.
	uint32_t hd_sector_count;		// Count of disk sectors (not Ext2 blocks) in use by this inode, not counting the actual inode structure nor directory entries linking to the inode.
	uint32_t flags;					// Flags (see below)
	uint32_t os_specific_value1;	// Operating System Specific value #1
	uint32_t ptr_blk[15];			// Block pointers
	// uint32_t ptr_blk0;				// Direct Block Pointer 0
	// uint32_t ptr_blk1;				// Direct Block Pointer 1
	// uint32_t ptr_blk2;				// Direct Block Pointer 2
	// uint32_t ptr_blk3;				// Direct Block Pointer 3
	// uint32_t ptr_blk4;				// Direct Block Pointer 4
	// uint32_t ptr_blk5;				// Direct Block Pointer 5
	// uint32_t ptr_blk6;				// Direct Block Pointer 6
	// uint32_t ptr_blk7;				// Direct Block Pointer 7
	// uint32_t ptr_blk8;				// Direct Block Pointer 8
	// uint32_t ptr_blk9;				// Direct Block Pointer 9
	// uint32_t ptr_blk10;				// Direct Block Pointer 10
	// uint32_t ptr_blk11;				// Direct Block Pointer 11
	// uint32_t single_ptr_blk;		// Singly Indirect Block Pointer (Points to a block that is a list of block pointers to data)
	// uint32_t double_ptr_blk;		// Doubly Indirect Block Pointer (Points to a block that is a list of block pointers to Singly Indirect Blocks)
	// uint32_t triple_ptr_blk;		// Triply Indirect Block Pointer (Points to a block that is a list of block pointers to Doubly Indirect Blocks)
	uint32_t gen_number;			// Generation number (Primarily used for NFS)
	uint32_t ext_attr_flag;			// In Ext2 version 0, this field is reserved. In version >= 1, Extended attribute block (File ACL).
	uint32_t sizeh;					// In Ext2 version 0, this field is reserved. In version >= 1, Upper 32 bits of file size (if feature bit set) if it's a file, Directory ACL if it's a directory
	uint32_t fragment_blk_addr;		// Block address of fragment
	uint8_t os_specific_value2[12];	// Operating System Specific Value #2
}Ext2_inode_t;



#endif