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

/**
#################
Struct      
#################

* filename       :  an array with the file to search
* root_dir       :  the system's root directory
* curr_dir       :  the current direcoty
* result         :  array of strings containing the results
* separator      :  OS's slash
* filters        :  array of filters
* result_length  :  result array length
* filter_length  :  filter array length
* result_size    :  result array block length
*/
typedef struct {
        long result_length, filter_length, result_index, result_size, win_disks_length;
        char *filename;
        char *root_dir;
        char *curr_dir;
        long *file_size;
        char *separator;
        char **win_disks;
        char **result;
        char **filters;
} Folder;   


extern void    init(Folder* folder, const char* filename);    /* Initialize struct's data */
extern void    find_file(Folder* folder);    /* Search the file in the disk */
extern int     add_filter(Folder* folder, char* new_filter);    /* Add words into filter array */
extern int     apply_filter(Folder* folder, char* filtered_result[], long filtered_file_size[], int flt_result_index);    /* Apply filters from filter array */
extern void    change_filename(Folder* folder , char* new_filename);    /* Change the file to be searched */
extern void    change_root_directory(Folder* folder , char* new_root_folder);    /* Change the root directory */
extern void    reset(Folder* folder);     /* Reset the struct */
extern int     print(Folder* folder);     /* Print data for debugging */
