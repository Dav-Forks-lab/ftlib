#include "../include/ftlib.h"

char* ftype(char* fname)
{
	char* tmp = fname;
	while(*(tmp++) != '.') {}
	
	return tmp;
}
