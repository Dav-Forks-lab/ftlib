#ifndef FTLIB_H
#define FTLIB_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

typedef struct _datafile DFILE;

extern DFILE* f_init();
extern void f_setname(DFILE* dfile, char* filename);
extern void f_setpath(DFILE* dfile, char* path);
extern void f_settype(DFILE* dfile, char* type);
extern void f_setsize(DFILE* dfile, long size);
extern void f_printdata(DFILE* dfile);

#endif