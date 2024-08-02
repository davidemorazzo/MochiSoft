cmake_minimum_required(VERSION 3.10)

# Nome del progetto

set(CMAKE_C_COMPILER "/home/codespace/opt/cross/bin/i686-elf-gcc")
set(CMAKE_CXX_COMPILER "/home/codespace/opt/cross/bin/i686-elf-g++")
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_ASM-ATT_COMPILER "/home/codespace/opt/cross/bin/i686-elf-as")

project(MochiOS)

set(PLATFORM_LAYOUT "/workspaces/MochiSoft/src/kernel/arch/i386/linker.ld")


add_subdirectory(src/libc)
add_subdirectory(src/kernel)

# Crea l'eseguibile
set(CMAKE_CXX_FLAGS "-ffreestanding -O2 -nostdlib -lgcc -T ${PLATFORM_LAYOUT}")
# set(CMAKE_CXX_FLAGS "-ffreestanding -O2 -nostdlib -T ${PLATFORM_LAYOUT}")

add_executable(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} kernel libc)
