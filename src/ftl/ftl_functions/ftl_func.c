#include "../ftl.h"
#include "ftl_struct.c"


DFILE* f_init()
{
	return (DFILE*)malloc(sizeof(struct _datafile));
}

void f_setname(DFILE* dfile, char* name)
{
	size_t nlen = sizeof(strlen(name) + 1); 
	dfile->fname = (char*)malloc(sizeof(char) * nlen);
	strcpy(dfile->fname, name);
}


void f_setpath(DFILE* dfile, char* path)
{
	size_t plen = sizeof(strlen(path) + 1); 
	dfile->fpath = (char*)malloc(sizeof(char) * plen);
	strcpy(dfile->fpath, path);
}


void f_settype(DFILE* dfile, char* type)
{
	size_t tlen = sizeof(strlen(type) + 1); 
	dfile->ftype = (char*)malloc(sizeof(char) * tlen);
	strcpy(dfile->ftype, type);
}


void f_setsize(DFILE* dfile, long size)
{
	dfile->fsize = size;
}


/* FOR DEBUG ONLY */
void f_printdata(DFILE* dfile)
{
	printf("%s - %s - %s - %ld", dfile->fname, dfile->fpath, dfile->ftype, dfile->fsize);
}

