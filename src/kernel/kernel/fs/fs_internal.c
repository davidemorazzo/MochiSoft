#include "kernel/fs/fs_internal.h"
#include "kernel/kglobals.h"

struct fs_state_s fs_state = {
	{0},
	{0},
	0,
	128
};

void init_fs(){
	AHCI_get_driver(&fs_state.driver);
}