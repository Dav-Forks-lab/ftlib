#include "./ftlib.h"


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
* size_array      :    Array containing files sizes

## string array type parameters ##
* win_disk_array       :    Array containing the windows disk letters
* result_array         :    Array containing the result filenames
* filter_array         :    Array containing the filters

*/
typedef struct _folder {
	long filter_length, result_length, result_index, result_fill_index, win_disk_length;
	char* filename;
	char* root_dir;
	char* curr_dir;
	char* separator;
	long* size_array;
	char** win_disk_array;
	char** result_array;
	char** filter_array;
} Folder;   
