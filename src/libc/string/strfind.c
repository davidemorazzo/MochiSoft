#include "string.h"

int strfind(char c, char* string, size_t size){
	for (int i = 0; i < size; i++){
		if (string[i] == c){
			return i;
		}	
	}
	return -1;
}