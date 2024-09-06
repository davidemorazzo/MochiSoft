# So CMake can tell whether or not it can process this file
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
project(libc)

# set(CMAKE_C_COMPILER "/home/codespace/opt/cross/bin/i686-elf-gcc")
# set(CMAKE_CXX_COMPILER "/home/codespace/opt/cross/bin/i686-elf-g++")
# set(CMAKE_C_COMPILER_WORKS 1)
# set(CMAKE_CXX_COMPILER_WORKS 1)


MESSAGE(STATUS "Building libc")

include_directories(include)
SET(CMAKE_CXX_FLAGS "-ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -g")
SET(CMAKE_C_FLAGS "-ffreestanding -O2 -Wall -Wextra -fno-exceptions -g")

set (libc_src
        ${PROJECT_SOURCE_DIR}/string/strlen.c
        ${PROJECT_SOURCE_DIR}/string/i32tohex.c
        ${PROJECT_SOURCE_DIR}/string/i32tobin.c
        ${PROJECT_SOURCE_DIR}/string/bytetohex.c
        ${PROJECT_SOURCE_DIR}/string/strcpy.c
)

add_library(libc SHARED OBJECT ${libc_src})

####### COMPILE FLAGS ##############

