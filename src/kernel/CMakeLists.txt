# So CMake can tell whether or not it can process this file
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
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

set (arch_i386_src 
        ${PROJECT_SOURCE_DIR}/arch/i386/microcode.cpp
        ${PROJECT_SOURCE_DIR}/arch/i386/tty.cpp
        ${PROJECT_SOURCE_DIR}/arch/i386/uart.cpp
        ${PROJECT_SOURCE_DIR}/arch/i386/gdt.cpp
        ${PROJECT_SOURCE_DIR}/arch/i386/idt.cpp
        # ${PROJECT_SOURCE_DIR}/arch/i386/PIC-8259.c

)

set (kernel_src
        ${PROJECT_SOURCE_DIR}/kernel/kernel.cpp)

set (bootloader_src
        ${PROJECT_SOURCE_DIR}/arch/i386/boot.s)

enable_language(ASM-ATT)

######### BOOTLOADER ##############
add_library(boot SHARED OBJECT ${PROJECT_SOURCE_DIR}/arch/i386/boot.s)


########### KERNEL ################
SET(CMAKE_CXX_FLAGS "-ffreestanding -O0 -Wall -Wextra -fno-exceptions -fno-rtti -g")
#target_compile_options(-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fnortti)

add_library(kernel SHARED OBJECT ${arch_i386_src} ${kernel_src})
# target_link_libraries(kernel libc) 

####### COMPILE FLAGS ##############


# SET_TARGET_PROPERTIES(kernel PROPERTIES LINK_FLAGS "-T ${PLATFORM_LAYOUT}")