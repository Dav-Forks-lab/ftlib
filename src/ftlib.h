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
        char *filename;
        char *root_dir;
        char *curr_dir;
        char **win_disks;
        char **result;
        long *file_size;
        char *separator;
        char **filters;
        long int result_length, filter_length, result_index, result_size, win_disks_length;
} Folder;   


extern void    init(Folder*, const char*);    /* Initialize struct's data */
extern void    find_file(Folder*);    /* Search the file in the disk */
extern int     add_filter(Folder*, char*);    /* Add words into filter array */
extern int     apply_filter(Folder*, char* [], long[], int);    /* Apply filters from filter array */
extern void    change_filename(Folder*, char*);    /* Change the file to be searched */
extern void    change_root_directory(Folder*, char*);    /* Change the root directory */
extern void    reset_directory(Folder*);    /* Reset root directory */
extern void    clear(Folder*);     /* Reset the struct */
extern int     print(Folder*);     /* Print data for debugging */
