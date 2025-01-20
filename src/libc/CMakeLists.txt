# So CMake can tell whether or not it can process this file
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)
project(libc)

# set(CMAKE_C_COMPILER "/home/codespace/opt/cross/bin/i686-elf-gcc")
# set(CMAKE_CXX_COMPILER "/home/codespace/opt/cross/bin/i686-elf-g++")
# set(CMAKE_C_COMPILER_WORKS 1)
# set(CMAKE_CXX_COMPILER_WORKS 1)


MESSAGE(STATUS "Building libc")

include_directories(include)
SET(CMAKE_CXX_FLAGS "-ffreestanding -O0 -Wall -Wextra -fno-exceptions -fno-rtti -g")
SET(CMAKE_C_FLAGS "-ffreestanding -O0 -Wall -Wextra -fno-exceptions -g")

set (libc_src
        # string.h
        ${PROJECT_SOURCE_DIR}/string/strlen.c
        ${PROJECT_SOURCE_DIR}/string/i32tohex.c
        ${PROJECT_SOURCE_DIR}/string/i32tobin.c
        ${PROJECT_SOURCE_DIR}/string/bytetohex.c
        ${PROJECT_SOURCE_DIR}/string/strcpy.c
        ${PROJECT_SOURCE_DIR}/string/itoa.c
        ${PROJECT_SOURCE_DIR}/string/trim.c
        ${PROJECT_SOURCE_DIR}/string/ltrim.c
        ${PROJECT_SOURCE_DIR}/string/rtrim.c
        ${PROJECT_SOURCE_DIR}/string/isspace.c
        ${PROJECT_SOURCE_DIR}/string/strfind.c

        # time.h
        ${PROJECT_SOURCE_DIR}/time/mktime.c
        ${PROJECT_SOURCE_DIR}/time/asctime.c
        ${PROJECT_SOURCE_DIR}/time/time.c
        ${PROJECT_SOURCE_DIR}/time/gmtime.c

        # stdio.h
        ${PROJECT_SOURCE_DIR}/stdio/snprintf.c
        ${PROJECT_SOURCE_DIR}/stdio/sprintf.c
        ${PROJECT_SOURCE_DIR}/stdio/printf.c
        ${PROJECT_SOURCE_DIR}/stdio/_va_snprintf.c
        ${PROJECT_SOURCE_DIR}/stdio/memcpy.c
)

add_library(libc SHARED OBJECT ${libc_src})

####### COMPILE FLAGS ##############

