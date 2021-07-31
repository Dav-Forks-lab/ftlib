#include "../header/ftlib.h"


/**
#################
Set filter
#################

* Fill the filter array with user given data
* return EXIT_SUCCESS if the filter is set
* return EXIT_FAILURE if the filter array if full 
*/
int add_filter(Folder *folder, char* new_filter)
{   
        if(folder->filter_length >= FILTER_LIMIT)
                return EXIT_FAILURE;
        
        folder->filters[folder->filter_length] = malloc(strlen(new_filter) + 1);
        strcpy(folder->filters[folder->filter_length], new_filter);
        folder->filter_length++;

        return EXIT_SUCCESS;
}


/**
#################
Apply filter
#################

* Fill a given pointer array with filter matching results
* Return filtered_result length
*/
int apply_filter(Folder *folder, char* filtered_result[], long filtered_file_size[], int flt_result_index)
{   
        for(int i=folder->result_index; i < folder->result_length; i++)
        {   
                for(int j=0; j < folder->filter_length; j++)
                {
                        /* Check if the result contains the filter */
                        if(strstr(folder->result[i], folder->filters[j]) != NULL)
                        {   
                                filtered_result[flt_result_index] = malloc(strlen(folder->result[i]) + 1);
                                strcpy(filtered_result[flt_result_index], folder->result[i]);
                                
                                filtered_file_size[flt_result_index] = folder->file_size[i];

                                flt_result_index++;
                                break;
                        }
                }
        }
        folder->result_index = folder->result_length;

        return flt_result_index;
}


/**
#################
Change filename
#################

* Set a new filename into the struct
*/
void change_filename(Folder* folder, char* new_filename)
{
        folder->filename = realloc(folder->filename, strlen(new_filename) + 1);
        strcpy(folder->filename, new_filename);
}


/*
#################
Change root dir
#################

* Set a new root directory into the struct
*/
void change_root_directory(Folder* folder, char* new_root_folder)               
{
        folder->root_dir = realloc(folder->root_dir, strlen(new_root_folder) +1);
        folder->curr_dir = realloc(folder->curr_dir, strlen(new_root_folder) +1);

        strcpy(folder->root_dir, new_root_folder);
        strcpy(folder->curr_dir, new_root_folder);
}


/**
#################
Clear
#################
*/
void reset(Folder* folder)
{
        free(folder->filename);
        free(folder->curr_dir);
        free(folder->root_dir);
        free(folder->file_size);
        free(folder->separator);
        
        for(int i=0; i < folder->result_length; i++)
                free(folder->result[i]);
        free(folder->result);

        #ifdef _WIN32
                for(int i=0; i < folder->win_disks_length; i++)
                        free(folder->win_disks[i]);
                free(folder->win_disks);
        #endif

        for(int i=0; i < folder->filter_length; i++)
                free(folder->filters[i]);
        free(folder->filters);
}
