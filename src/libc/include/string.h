#ifndef INCLUDE_STRING_H
#define INCLUDE_STRING_H 1

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t strlen(const char *);
void strcpy(char * source, char * dest);
void strcpyn(char * source, char * dest, size_t n);

void i32tohex(uint32_t i, char s[9]);
void bytetohex(uint8_t i, char s[3]);
void i32tobin(uint32_t i, char s[33]);
void itoa(int i, char s[19]);

#ifdef __cplusplus
}
#endif

#endif