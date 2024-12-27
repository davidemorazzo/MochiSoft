# MochiSoft

Repository con il codice sorgente del sistema operativo MochiSoft OS. Lo sviluppo di questo kernel è basato sulla CPU ```Intel 80386``` (i386) che viene emulata con altre periferiche da QEMU.

## Usage
Il build system è CMAKE, quindi per compilare l'immagine del kernel per architettura i386 si procede con:

1. Clonare la repository
	``` bash
	git clone https://github.com/davidemorazzo/MochiSoft.git
	cd MochiSoft
	```
2. Compilare l'immagine ( che si troverà in `build/MochiOS`)
	```bash
	mkdir -p build && cd build
	cmake ..
	make
	```

Per eseguire l'emulazione con QEMU, aprire la cartella `MochiSoft` con VScode (nella cartella `.vscode` sono presenti i file di configurazione del debug). La configurazione di debug `Launch QEMU-i386 MochiOS` avvierà un'istanza di 'qemu-system-i386' con aggangiato GDB per il debug con VScode

## QEMU configuration
La configurazione del sistema emulato:

1. VGA disattivata
2. Comunicazione solo attraverso UART0
3. Debugger GDB esposto sulla porta `localhost:1234`
4. Disco rigido con controller AHCI su SATA (immagine del disco `hdd.img` in formato qcow2 di QEMU)

# Features
| Oggetto								| Stato	|
|---									| :---:	|
|Basic UART driver						| ✅️	|
|Kernel heap allocator					| 🛠️	 |
|Real Time clock						| ✅️	|
|PIT/APIC								| 🛠️ 	 |
|Driver AHCI per dispositivi su SATA	| ✅️	|
|Filesystem driver for Ext2				| ❌️ 	|
|Paging									| ❌️	|
|Threads								| ❌️	|
|Multi-processing						| ❌️	|
|tty and Shell interpreter				| ❌️	|
|VGA driver								| ❌️	|
