#include "string.h"

void bytetohex(uint8_t i, char s[3]){

	for (int it=0; it<2; it++){
		s[1-it] = ( (i >> (it*4)) & 0xF ) + '0';
		if (s[1-it] > '9'){
			s[1-it] +=7;
		}
	}
	s[2] = '\0';
}