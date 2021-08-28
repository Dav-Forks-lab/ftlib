#ifndef FTLIB_H_
#define FTLIB_H_

/**
#################
Include libraries
#################
*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>      //Used in the print function (printf)
#include <dirent.h>

#ifdef  _WIN32
	#include <windows.h>
#endif

#define FILTER_LIMIT 32


typedef struct _folder Folder;

extern Folder* init();    /* Initialize struct's data */
extern void    search(Folder* folder);    /* Search the file in the disk */
extern int     add_filter(Folder* folder, char* new_filter);    /* Add words into filter array */
extern int     app_filter(Folder* folder, char* flt_result_array[], long flt_size_array[], int flt_index);    /* Apply filters from filter array */
extern void    set_filename(Folder* folder , char* new_filename);    /* Set the filename to be searched */
extern void    set_root_directory(Folder* folder , char* new_root_folder);    /* Set another root dir, different from the stock one */
extern char*   lowercase(char* string);
extern void    reset(Folder* folder);     /* Reset the struct to init data */
extern void    print(Folder* folder);     /* Print data for debugging */

#endif