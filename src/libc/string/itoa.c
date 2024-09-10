/*Davide Morazzo - Settembre 2024

itoa -> integer base 10 to alpha (string)
La stringa ritornata avrà lunghezza massima di 19 (MAX_INT64) + 1 ('\0') */

#include "string.h"
#include <stdbool.h>

void itoa(int i, char s[19]){
	bool is_neagtive = i < 0;
	char buffer[19];
	int buf_idx = 0;
	int i_cpy;

	if (is_neagtive){
		i_cpy = -1 * i;
	}else{
		i_cpy = i;
	}

	if (i==0) {
		/*Trivial case*/
		strcpy("0\0", s);

	}else{
		while(i_cpy > 0){
			buffer[buf_idx] = (i_cpy % 10) + '0';
			i_cpy = i_cpy / 10;
			buf_idx++;
		}

		if (is_neagtive){
			buffer[buf_idx] = '-';
			buf_idx++;
		}
		// buffer[buf_idx] ='\0';
		// Reverse string
		// for(int k=buf_idx; k>=0; k--){
		// 	s[19-k] = buffer[k]; is_neagtive
		// }
		buf_idx--;
		int k=0;
		while(buf_idx >= 0){
			s[k] = buffer[buf_idx];
			k++;
			buf_idx--;
		}
		s[k] = '\0';
	}	
}