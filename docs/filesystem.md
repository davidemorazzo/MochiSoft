# Filesystem di MochiOS

## Obiettivo
Avviare qemu con un'immagine di un disco e poter interagire con i file su questo disco virtuale. 

## Architettura
Interfaccia principale da usare nel kernel contenuta in `include/kernel/fs.h`. Questa interfaccia deve essere indipendente dal _driver_ che viene usato. Inizialmente il primo driver da implementare è _Ext2_.
```txt
+-----+   +-----------+   +-------------+   +------+
| HDD |<=>| ATA iface |<=>| Ext2 driver |<=>| fs.h |
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

## Driver interface
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

## ATA interface
https://wiki.osdev.org/ATA_PIO_Mode
> Su QEMU per impostart l'HardDisk aggiungere `-hda <path/to/disk.img>`