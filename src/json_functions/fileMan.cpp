#include <iostream>
#include <fstream>

#include "../include/ftlib.h"
#include "../ftl/ftl.h"

void writefile(DFILE* file)
{
	int prova;
}


DFILE* readfile(char* filename, char* fname)
{
	std::ifstream i("prova.json");
	json obj;
	i >> obj;

	std::cout << obj;
}