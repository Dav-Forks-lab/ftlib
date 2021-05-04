/*
    INTESTAZIONE
*/


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
* result_lenght  :  result array lenght
* filter_lenght  :  filter array lenght
* result_size    :  result array block lenght
*/
typedef struct {
        char *filename;
        char *initial_root_dir;
        char *root_dir;
        char *curr_dir;
        char **win_disks;
        char **result;
        char *separator;
        char **filters;
        long int result_lenght, filter_lenght, result_size, win_disks_lenght;
        int linux_multi_disk_search;
} Folder;   

/**
#################
Init
#################

* Initialize struct's data
*/
void init(Folder *folder, const char* filename) 
{      
        /*
        * If OS is Windows an array is filled with all of the usable drives
        * The find_file function doesn't search in all of those automatically
        * The choose is given to the developer
        * Searching in all of the disks can be easly done by loop through the array
        * and use the change_root_directory with find_file function
        * The default disk is set to "C:\"
        */
        #ifdef _WIN32

                char driver_array[MAX_PATH] = {0};
                DWORD dword_size = MAX_PATH;
                DWORD dword_result = GetLogicalDriveStrings(dword_size, driver_array);

                /* Allocate multiple disk array size */
                folder->win_disks = malloc(sizeof(char*) * 32);
                folder->win_disks_lenght = 0;

                if (dword_result > 0 && dword_result <= MAX_PATH)
                {
                        char* drive = driver_array;
                        while(*drive)
                        {       
                                folder->win_disks[folder->win_disks_lenght] = malloc(strlen(drive) + 1);
                                strcpy(folder->win_disks[folder->win_disks_lenght], drive);

                                // Point to the next drive
                                drive += strlen(drive) + 1;
                                folder->win_disks_lenght++;
                        }
                }

                /* Setting the defualt disk and the default separator */
                folder->initial_root_dir = malloc(strlen("C:\\") + 1);
                strcpy(folder->initial_root_dir, "C:\\");
                folder->root_dir = malloc(strlen("C:\\") + 1);
                strcpy(folder->root_dir, "C:\\");
                folder->separator = malloc(strlen("\\") + 1);
                strcpy(folder->separator, "\\");

        /*
        * Linux based OS just need "/" as base folder
        * Other mouted disks are reachable from there
        */
        #elif __linux__
                folder->initial_root_dir = malloc(strlen("C:\\") + 1);
                strcpy(folder->initial_root_dir, "C:\\");
                folder->root_dir = malloc(strlen("/") + 1);
                strcpy(folder->root_dir, "/");
                folder->separator = malloc(strlen("/") + 1);
                strcpy(folder->separator, "/");
                /* By deault it avoid /dev folder, it search only in the main disk */
                folder->linux_multi_disk_search = 0;

        #endif       

         /* Init sizes */
        folder->result_size = 1024;
        folder->result_lenght = 0;
        folder->filter_lenght = 0;

        folder->filters = malloc(FILTER_LIMIT * sizeof(char *));
        folder->result = malloc(folder->result_size * sizeof(char *));
        folder->filename = malloc(strlen(filename) + 1);
        folder->curr_dir = malloc(strlen(folder->root_dir) + 1);

        strcpy(folder->filename, filename);    
        strcpy(folder->curr_dir, folder->root_dir);
}

/**
#################
Find file
#################

* Recursive search through file directories 
*/
void find_file(Folder *folder)
{       
        char* directory = malloc(strlen(folder->curr_dir) + 1);
        strcpy(directory, folder->curr_dir);

        DIR *dir;
        struct dirent *ent;

        if(strchr(directory, '.') != NULL)
                return; 

        /* Try to open the directory */
        if((dir = opendir(directory)) != NULL)
        {     
                /* Search in every sub-folder */
                while((ent = readdir(dir)) != NULL)
                {       
                        #ifdef __linux__
                                /* dev/fd folder not needed for the file searching */	
                                if(strstr(ent->d_name, "fd")   != NULL ||
                                   strstr(ent->d_name, "proc") != NULL ||
                                   strstr(ent->d_name, "dev")  != NULL || 
                                   strstr(ent->d_name, "tmp")  != NULL
                                   )
                                        continue;	
                                
                                /* If multi_disk_search is disable it avoids to search in other disks */
                                if(!folder->linux_multi_disk_search && strstr(ent->d_name, "mnt") != NULL)
                                        continue;

                        #elif   _WIN32
                                if(strchr(ent->d_name, '$') != NULL)
                                        continue;
                                        
                        #endif

                        if(folder->result_lenght == folder->result_size)
                        {   
                                int old_size = folder->result_size;
                                folder->result_size *= 2;
                                
                                char **template = malloc(folder->result_size * sizeof(char *));
                                memcpy(template, folder->result, old_size * sizeof(char *));
                                memset(folder->result, 0, old_size * sizeof(char *));
                                free(folder->result);
                                folder->result = template;
                        }

                        if(strstr(ent->d_name, folder->filename) != NULL)
                        {       
                                /* Set the necessary space for the dir */
                                folder->result[folder->result_lenght] = malloc(strlen(directory) + strlen(ent->d_name) +1);
                                /* Assemble the dir string */
                                strcpy(folder->result[folder->result_lenght], directory);
                                strcat(folder->result[folder->result_lenght], ent->d_name);
                                /* Increase the dir index */
                                folder->result_lenght++;
                        }

                        /* Set current directory */
                        folder->curr_dir = realloc(folder->curr_dir, strlen(directory) + strlen(ent->d_name) + strlen(folder->separator) + 2);
                        strcpy(folder->curr_dir, directory);
                        strcat(folder->curr_dir, ent->d_name);
                        strcat(folder->curr_dir, folder->separator);
   
                        /* Start the search again with recursion */
                        find_file(folder);
                }
                closedir(dir);
        }
}


/**
#################
Set filter
#################

* Fill the filter array with user given data
*/
void set_filter(Folder *folder, char* new_filter[], int filter_len)
{   
        for(int i = 0; i < filter_len; i++)
        {   
                if(i < FILTER_LIMIT)
                {   
                        folder->filters[i] = malloc(strlen(new_filter[i]) + 1);
                        strcpy(folder->filters[i], new_filter[i]);
                        folder->filter_lenght++;
                }
                else
                        break;
        }
}

/**
#################
Apply filter
#################

* Fill a given pointer array with filter matching results
*/
void apply_filter(Folder *folder, char* filtered_result[], int* index)
{   
        for(int i=0; i < folder->result_lenght; i++)
        {   
                for(int j=0; j < folder->filter_lenght; j++)
                {
                        /* Check if the result contains the filter */
                        if(strstr(folder->result[i], folder->filters[j]) != NULL)
                        {   
                                filtered_result[*index] = malloc(strlen(folder->result[i]) +1);
                                strcpy(filtered_result[*index], folder->result[i]);
                                
                                (*index)++;
                                break;
                        }
                }
        }
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

/**
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

void resert_directory(Folder* folder)
{
        folder->root_dir = realloc(folder->root_dir, strlen(folder->initial_root_dir) +1);
        folder->curr_dir = realloc(folder->curr_dir, strlen(folder->initial_root_dir) +1);

        strcpy(folder->root_dir, folder->initial_root_dir);
        strcpy(folder->curr_dir, folder->initial_root_dir);
}

/**
#################
Print result
#################

* Loop through the result array and prints the results
* Made for dev
*/
int print(Folder* folder)
{   
        /* Check if the array is non empty */
        if(folder->result[0])
                /* Print array data */
                for(int i=0; i < folder->result_lenght; i++)
                        printf("%s\n", folder->result[i]);
        else 
                /* If the array is empty return an error code */
                return 1;

        return 0;
}
