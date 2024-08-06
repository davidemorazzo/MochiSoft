#include <string.h>

void i32tohex(uint32_t i, char s[9]){
	/* FF FF FF FF */

	for(int it=7; it>=0; it--){
		s[7-it] = ( (i >> (it*4)) & 0xF ) + '0';
		if (s[7-it] > '9'){
			s[7-it] +=7;
		}
	}

	s[8]='\0';

	// return s;
}