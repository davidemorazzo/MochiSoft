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

include_directories(include/kernel)

set (arch_i386_src 
        ${PROJECT_SOURCE_DIR}/arch/i386/inb.cpp
        ${PROJECT_SOURCE_DIR}/arch/i386/outb.cpp
        ${PROJECT_SOURCE_DIR}/arch/i386/tty.cpp
        ${PROJECT_SOURCE_DIR}/arch/i386/uart.cpp)

set (kernel_src
        ${PROJECT_SOURCE_DIR}/kernel/kernel.cpp)

set (bootloader_src
        ${PROJECT_SOURCE_DIR}/arch/i386/boot.s)

enable_language(ASM-ATT)

######### BOOTLOADER ##############
add_library(boot OBJECT ${PROJECT_SOURCE_DIR}/arch/i386/boot.s)


add_library(kernel SHARED OBJECT ${arch_i386_src} ${kernel_src})
target_link_libraries(kernel libc)

####### COMPILE FLAGS ##############

#SET(CMAKE_ASM-ATT_COMPILE_OBJECT 
#  "<CMAKE_ASM-ATT_COMPILER> -o <OBJECT> <SOURCE>")

SET(CMAKE_CXX_FLAGS "-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti")

SET_TARGET_PROPERTIES(kernel PROPERTIES LINK_FLAGS 
  "-T ${PLATFORM_LAYOUT}")