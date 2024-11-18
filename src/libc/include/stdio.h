/* Davide Morazzo - Settembre 2024 */
#ifndef INCLUDE_STDIO_H
#define INCLUDE_STDIO_H 1

#include <stddef.h>
#include <stdarg.h>

#define stdin 0
#define stdout 1
#define stderr 2

int snprintf ( char * s, size_t n, const char * format, ... );
int sprintf ( char * str, const char * format, ... );
int _va_snprintf(char * s, size_t n, const char * format, va_list argptr);

#endif