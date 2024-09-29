#ifndef INCLUDE_KERNEL_KSTDIO_H
#define INCLUDE_KERNEL_KSTDIO_H 1

#define STDIO_PORT UART0
void kprint(const char * format, ...);

#endif