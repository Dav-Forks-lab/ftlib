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

## long type parameters ##
* filter_length        :    The length of the filter array
* result_length        :    The length of the result array
* rsult_index          :    The current index in the result array
* result_fill_index    :    The index who indicates the last item in the result array 
* win_disks_length     :    The length of the windows disks array

## string type parameters ##
* filename             :    String with the file to be searched
* root_dir             :    String with the root directory
* curr_dir             :    String with the current directory during file search
* separator            :    String path separator, OS dependet

## long array parameters ##
* file_size_array      :    Array containing files sizes

## string array type parameters ##
* win_disk_array       :    Array containing the windows disk letters
* result_array         :    Array containing the result filenames
* filter_array         :    Array containing the filters

*/
typedef struct {
        long filter_length, result_length, result_index, result_fill_index, win_disk_length;
        char* filename;
        char* root_dir;
        char* curr_dir;
        char* separator;
        long* file_size_array;
        char** win_disk_array;
        char** result_array;
        char** filter_array;
} Folder;   


extern void    init(Folder* folder, const char* filename);    /* Initialize struct's data */
extern void    find_file(Folder* folder);    /* Search the file in the disk */
extern int     add_filter(Folder* folder, char* new_filter);    /* Add words into filter array */
extern int     apply_filter(Folder* folder, char* filtered_result[], long filtered_file_size[], int flt_result_index);    /* Apply filters from filter array */
extern void    change_filename(Folder* folder , char* new_filename);    /* Change the file to be searched */
extern void    change_root_directory(Folder* folder , char* new_root_folder);    /* Change the root directory */
extern void    reset(Folder* folder);     /* Reset the struct */
extern int     print(Folder* folder);     /* Print data for debugging */
