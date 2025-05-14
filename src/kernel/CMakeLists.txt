# So CMake can tell whether or not it can process this file
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
# set(CMAKE_C_COMPILER "/home/codespace/opt/cross/bin/i686-elf-gcc")
# set(CMAKE_CXX_COMPILER "/home/codespace/opt/cross/bin/i686-elf-g++")
# set(CMAKE_C_COMPILER_WORKS 1)
# set(CMAKE_CXX_COMPILER_WORKS 1)
# set(CMAKE_ASM-ATT_COMPILER "/home/codespace/opt/cross/bin/i686-elf-as")
project(kernel)

# set(PLATFORM_LAYOUT "arch/i386/linker.ld")
MESSAGE(STATUS "Building Kernel")

# include_directories(include/kernel)
include_directories(include)
include_directories(${libc_SOURCE_DIR}/include )
include_directories(${PROJECT_SOURCE_DIR}/src/kernel/include)
enable_language(ASM-ATT)

set (arch_i386_src 
${PROJECT_SOURCE_DIR}/arch/i386/microcode.c
${PROJECT_SOURCE_DIR}/arch/i386/uart.c
${PROJECT_SOURCE_DIR}/arch/i386/gdt.c
${PROJECT_SOURCE_DIR}/arch/i386/idt.c
${PROJECT_SOURCE_DIR}/arch/i386/8259/PIC-8259.c
${PROJECT_SOURCE_DIR}/arch/i386/8253/PIT.c
# ${PROJECT_SOURCE_DIR}/arch/i386/PIC-8259.c
${PROJECT_SOURCE_DIR}/arch/i386/pagetable.c
)

set (kernel_src
        ${PROJECT_SOURCE_DIR}/kernel/kernel.c
        ${PROJECT_SOURCE_DIR}/kernel/exceptions.c
        ${PROJECT_SOURCE_DIR}/kernel/kstdio.c
        ${PROJECT_SOURCE_DIR}/kernel/kglobals.c
        ${PROJECT_SOURCE_DIR}/kernel/ISR/exc_isr_wrapper.s
        ${PROJECT_SOURCE_DIR}/kernel/ISR/IRQ_wrapper.s
        ${PROJECT_SOURCE_DIR}/kernel/ISR/IRQ_handlers/irq0.c
        ${PROJECT_SOURCE_DIR}/kernel/ISR/IRQ_handlers/irq4.c
        ${PROJECT_SOURCE_DIR}/kernel/ISR/IRQ_handlers/irq8.c
        ${PROJECT_SOURCE_DIR}/kernel/ISR/IRQ_handlers/irq11.c
        ${PROJECT_SOURCE_DIR}/kernel/memory/waterMarkAllocator.c
        ${PROJECT_SOURCE_DIR}/kernel/memory/paging.c
        ${PROJECT_SOURCE_DIR}/kernel/memory/page_allocator.c
        ${PROJECT_SOURCE_DIR}/kernel/dev/RTC.c
        ${PROJECT_SOURCE_DIR}/kernel/dev/init_devs.c
        ${PROJECT_SOURCE_DIR}/kernel/ISR/syscall_0x80.c
        ${PROJECT_SOURCE_DIR}/kernel/ISR/system_calls.s
        ${PROJECT_SOURCE_DIR}/kernel/syscall/sys_time.c
        ${PROJECT_SOURCE_DIR}/kernel/syscall/sys_write.c
        ${PROJECT_SOURCE_DIR}/kernel/syscall/sys_read.c
        ${PROJECT_SOURCE_DIR}/kernel/modules/tty.c
        ${PROJECT_SOURCE_DIR}/kernel/dev/PCI.c
        ${PROJECT_SOURCE_DIR}/kernel/dev/AHCI.c
        ${PROJECT_SOURCE_DIR}/kernel/fs/ext2/ext2.c
        ${PROJECT_SOURCE_DIR}/kernel/proc/sys_create_process.c
        ${PROJECT_SOURCE_DIR}/kernel/proc/proc_register.c
        ${PROJECT_SOURCE_DIR}/kernel/proc/proc_get.c
        ${PROJECT_SOURCE_DIR}/kernel/proc/proc_exit.c
        ${PROJECT_SOURCE_DIR}/kernel/proc/sched_isr.s
        ${PROJECT_SOURCE_DIR}/kernel/proc/sched_callback.c
        ${PROJECT_SOURCE_DIR}/kernel/proc/sched_init.c

        ${PROJECT_SOURCE_DIR}/linked_list/list.c
        ${PROJECT_SOURCE_DIR}/linked_list/list_node.c
        ${PROJECT_SOURCE_DIR}/linked_list/list_iterator.c
)

set (bootloader_src
        ${PROJECT_SOURCE_DIR}/arch/i386/boot.s
)


######### BOOTLOADER ##############
SET(CMAKE_ASM-ATT_FLAGS "-g -O0")

########### KERNEL ################
SET(CMAKE_CXX_FLAGS "-ffreestanding -O0 -Wall -Wextra -fno-exceptions -fno-rtti -g")
SET(CMAKE_C_FLAGS "-ffreestanding -O0 -Wall -Wextra -g")
#target_compile_options(-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fnortti)

add_library(kernel SHARED OBJECT ${bootloader_src} ${arch_i386_src} ${kernel_src})
# target_link_libraries(kernel libc) 

####### COMPILE FLAGS ##############


# SET_TARGET_PROPERTIES(kernel PROPERTIES LINK_FLAGS "-T ${PLATFORM_LAYOUT}")