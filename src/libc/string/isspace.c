#include "string.h"

unsigned short int isspace(char s){
	if (
		s != ' ' &&
		s != '\t' &&
		s != '\n' &&
		s != '\v' &&
		s != '\f' &&
		s != '\r'){
		return 0;
	}else{
		return 1;
	}
}