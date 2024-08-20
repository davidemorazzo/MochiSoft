# MochiSoft OS project

Obiettivi del progetto:

1. Sviluppare da zero il kernel  [source](https://www.quora.com/What-are-some-interesting-operating-system-projects-at-the-college-level)
    1. Il kernel deve essere di tipo _monolythic_ per semplicità
    2. Il kernel deve gestire la memoria
    3. Il kernel deve interfacciarsi con i dispositivi hardware (emulati?)
       1. Sviluppo driver per UART 16550 - **In corso** 
    4. Il kernel deve gestire thread e processi per il multitasking
    5. Il kernel deve fornire un'interfaccia testuale per l'utente
       1. Sviluppo di un terminale seguendo lo standard [per un'implementazione base](https://wiki.osdev.org/Terminals)
    6. Il kernel deve fornire degli API ai programmi che esegue
    7. Il kernel deve essere eseguito in un ambiente virtuale (qemu), ed eseguire il boot
       1. Esecuzione usando qemu, che richiama la funzione di entry point del kernel `kernel_main`
    8. Sviluppare utility di base nel modulo `src/utils` da usare in tutti i moduli, oppure capire come includere le librerie standard di CPP
    9. Il kernel deve avere un modulo che fa logging

2. Configurare il bootloader 
    1. Deve inizializzare l'hardware
    2. deve eseguire l'entry point del sistema operativo (kernel). [Lista funzioni BIOS](https://wiki.osdev.org/BIOS#Common_functions)
    3. Usare bootloader GRUB per caricare entry point del kernel oppure sviluppare bootloader [bare bone](https://wiki.osdev.org/Bare_Bones)


## Ambiente
L'ambiete di esecuzione del kernel è la simulazione di un sistema X86_64 con qemu. Essendo sviluppato in codespaces voglio solamente interagire tramite linea di comando quindi devo usare la modalità senza grafica. Qemu crea una porta seriale che fa passare sulla console di VScode. 

Per eseguire qemu:
```bash
sudo apt install qemu-system
qemu-system-x86_64 -nographic -kernel build/MochiOS
```

Per uscire da qemu 
```
Ctrl-A x
``` 

# Compilazione
Ho compilato una toolchain per il cross-compiling nella cartella `$HOME/opt/cross`:
```
/home/codespace/opt/cross
└── bin
    ├── i686-elf-addr2line
    ├── i686-elf-ar
    ├── i686-elf-as
    ├── i686-elf-c++
    ├── i686-elf-c++filt
    ├── i686-elf-cpp
    ├── i686-elf-elfedit
    ├── i686-elf-g++
    ├── i686-elf-gcc
    ├── i686-elf-gcc-14.1.0
    ├── i686-elf-gcc-ar
    ├── i686-elf-gcc-nm
    ├── i686-elf-gcc-ranlib
    ├── i686-elf-gcov
    ├── i686-elf-gcov-dump
    ├── i686-elf-gcov-tool
    ├── i686-elf-gprof
    ├── i686-elf-ld
    ├── i686-elf-ld.bfd
    ├── i686-elf-lto-dump
    ├── i686-elf-nm
    ├── i686-elf-objcopy
    ├── i686-elf-objdump
    ├── i686-elf-ranlib
    ├── i686-elf-readelf
    ├── i686-elf-size
    ├── i686-elf-strings
    └── i686-elf-strip
```

# Roadmap

1. ✅ Bootloader
2. ✅ Global Descriptor Table
3. ✅ Interrupt Descriptor Table
4. 🛠️ **Programmable Interrupt Controller 8259 setup**
5. Interrupt Service Routine for COM1 
6. TTY terminal
7. Timers setup
8. Time features
9. Tasks

<!-- 1. Sistema operativo che supporti l'esecuzione di task in multithreading
    1. Come obiettivo poter eseguire semplici ELF
2. Interazione con l'utente attraverso linea di comando, stile bash
3. Nella prima fase deve essere un processo all'interno di linux. Possibile evoluzione per essere eseguito su qemu o simili

Moduli del progetto da costruire per l'OS:

- **Kernel**
    - **Scheduler**
    - **Memory manager**
    - **File system** (da mettere nel kernel o come modulo separato?)
    - **Resource manager**
- **CLI user interface** -->