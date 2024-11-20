#ifndef INCLUDE_KERNEL_KSTDIO_H
#define INCLUDE_KERNEL_KSTDIO_H 1

#include "kernel/syscall.h"
#include "stdio.h"

#define STDIO_PORT UART0
void kprint(const char * format, ...);

#define KLOGINFO(...) \
sys_write(stdout, "\x1b[36m" "<INFO> ", 12); \
kprint(__VA_ARGS__); \
sys_write(stdout, "\n\x1b[0m", 5);

#define KLOGERROR(...) \
sys_write(stdout, "\x1b[31m" "<ERROR> ", 13); \
kprint(__VA_ARGS__); \
sys_write(stdout, "\n\x1b[0m", 5);

#define KLOGWARN(...) \
sys_write(stdout, "\x1b[33m" "<WARNING> ", 15); \
kprint(__VA_ARGS__); \
sys_write(stdout, "\n\x1b[0m", 5);


#endif