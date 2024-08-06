#include <string.h>

void i32tobin(uint32_t i, char s[33]){
	for (int it = 32 - 1; it >= 0; it--) {
		s[it] = (i & 1) + '0';
		i >>= 1;
  	}

	s[32] = '\0';
}

