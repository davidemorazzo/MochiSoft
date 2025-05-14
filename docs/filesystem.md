# Filesystem di MochiOS

## Obiettivo
Avviare qemu con un'immagine di un disco e poter interagire con i file su questo disco virtuale. 

## Architettura
Interfaccia principale da usare nel kernel contenuta in `include/kernel/fs.h`. Questa interfaccia deve essere indipendente dal _driver_ che viene usato. Inizialmente il primo driver da implementare è _Ext2_.
```txt
+-----+   +-----------+   +-------------+   +------+
| HDD |<=>|    AHCI   |<=>| Ext2 driver |<=>| fs.h |
+-----+   +-----------+   +-------------+   +------+
```

## Requisiti
1. Monitorare lo spazio libero
2. Monitorare il collegamento tra blocks e file
3. Creare nuovi file
	- 3.1 Organizzare i file in strutture di cartelle
	- 3.2 Buffering di scrittura per diminuire le op. sul disco
	- 3.3 Poter assegnare un file la modalità read only
4. Leggere da un qualsiasi file
	- 4.1 Buffering di lettura per diminuire le operazioni sul disco
	- 4.2 Caching dei file letti frequentemente in RAM
	- 4.3 Access control sui file
5. Aggiornare un qualsiasi file
	- 5.1 Rinominare un file
	- 5.2 Buffering di scritture per diminuire le op. sul disco
	- 5.3 Poter assegnare un file la modalità read only
6. Cancellare un qualsiasi file

## Interfacce
### Interfaccia Filesystem / Kernel 
Interfaccia esposta da fs.h. Intesa solo per uso dentro al kernel. 
Usata dalle funzionalità di sistema sys_* che sono anche mappate
sulle system calls (es. sys_read, sys_write). 
```c
struct {
	int    (*init)       ( ... );
	int    (*lockFile)	 ( ... );
	int    (*unlockFile) ( ... );
	int    (*createFile) ( ... );
	void * (*readFile)   ( ... );
	int    (*updateFile) ( ... );
	int    (*removeFile) ( ... );

} fsDriver;
```

### Interfaccia device-driver / Filesystem
Interfaccia per interagire direttamente con l'hardware (HDD, floppy ...). Attualmente prevede solo lettura e scrittura. 
```c
struct {
	const int total_sectors;		// Total n. of addressable sectors
	const int sector size;		// Size in bytes of 1 sector
	int (*write)(uint32_t startl, uint32_t starth, uint32_t count, void * buf);
	int (*read)(uint32_t startl, uint32_t starth, uint32_t count, void * buf);
}
```

## AHCI controller
Per interagire in modo efficiente con le periferiche di storage, usare il controller AHCI, che controlla la periferica SATA con i comandi ATAPI o SCSI e rende disponibile l'API direttamente in memoria o su particolari porte I/O. L'interazione con il controller AHCI è molto legata al bus PCI. 
```txt
+-----------+   +--------+   +--------+   +----+
| SATA DEV. |<=>|  AHCI  |<=>| RAM/IO |<=>| OS | 
+-----------+   +--------+   +--------+   +----+
```

Le porte I/O per interagire con il controller `PCI`: 
- `PCI_ADDR` = 0xCF8
- `PCI_DATA` = 0xCFC

```txt
Register	Offset	Bits 31-24	Bits 23-16	Bits 15-8	Bits 7-0
0x0			0x0	    Device ID				Vendor ID
0x1			0x4		Status					Command
0x2			0x8		Class code	Subclass	Prog IF		Revision ID
0x3			0xC		BIST		Header type	Latency Tmr	Cache Line Size
...
```

Alcuni link utili:
- https://github.com/fysnet/FYSOS/blob/39da816ac995fe246853c062dd1e148bb0d6f083/main/usb/utils/include/pci.h#L190
- https://forum.osdev.org/viewtopic.php?t=41833
- https://wiki.osdev.org/PCI#Enumerating_PCI_Buses
- https://wiki.osdev.org/AHCI
  
## ATA interface
https://wiki.osdev.org/ATA_PIO_Mode

http://users.utcluj.ro/~baruch/media/siee/labor/ATA-Interface.pdf
https://tc.gts3.org/cs3210/2016/spring/r/hardware/ATA8-ACS.pdf
https://hddguru.com/download/documentation/ATA-ATAPI-standard-6/ATA-ATAPI-6.pdf

> Su QEMU per impostart l'HardDisk aggiungere `-hda <path/to/disk.img>`


# Funzioni FS

## `void init_fs()`

## ` file_desc fs_open_file(char * path)`
mette un lock sul file e lo mette in memoria heap del kernel, ritornando un
file descriptor usabile per leggere il file. Operazioni da fare:

1. Trovare file nel filesystem
	1. Trovare inode con `ext2_inode_from_path`
2. Mettere lock su struttura `fs_state.file_handler_array`
	1. Bloccare struttura `file_handler_array` con flag `fs_state.array_lock` usando test&set. 
	2. Scansionare `file_handler_array` cercando se stesso file esiste già aperto. In questo 
	caso ritornare un file descriptor invalido => 0.
	3. Impostare `file_handler_array.open=1`
	4. Sbloccare `file_handler_array` con flag `fs_state.array_lock=0`
3. Leggere file e immagazzinarlo su `fs_state.file_handler_array`
	1. Leggere il contenuto del file con `ext2_read_inode_blocks`
4. Ritornare descrittore file