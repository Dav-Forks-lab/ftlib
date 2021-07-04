/*
    INTESTAZIONE
*/

/**
#################
Include libraries
#################
*/

#include "ftlib.h"

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
                folder->win_disks_length = 0;

                if (dword_result > 0 && dword_result <= MAX_PATH)
                {
                        char* drive = driver_array;
                        while(*drive)
                        {       
                                folder->win_disks[folder->win_disks_length] = malloc(strlen(drive) + 1);
                                strcpy(folder->win_disks[folder->win_disks_length], drive);

                                // Point to the next drive
                                drive += strlen(drive) + 1;
                                folder->win_disks_length++;
                        }
                }

                /* Setting the defualt disk and the default separator */
                folder->root_dir = malloc(strlen("C:\\") + 1);
                strcpy(folder->root_dir, "C:\\");
                folder->separator = malloc(strlen("\\") + 1);
                strcpy(folder->separator, "\\");

        /*
        * Linux based OS just need "/" as base folder
        * Other mouted disks are reachable from there
        */
        #elif __linux__
                folder->root_dir = malloc(strlen("/") + 1);
                strcpy(folder->root_dir, "/");
                folder->separator = malloc(strlen("/") + 1);
                strcpy(folder->separator, "/");

        #endif       

         /* Init sizes */
        folder->result_size = 128;
        folder->result_length = 0;
        folder->filter_length = 0;
        folder->result_index = 0;

        folder->filters = malloc(FILTER_LIMIT * sizeof(char *));
        folder->result = malloc(folder->result_size * sizeof(char *));
        folder->file_size = malloc(folder->result_size * sizeof(long));
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
                                if(strcmp(ent->d_name, "fd")   == 0 ||
                                   strcmp(ent->d_name, "proc") == 0 ||
                                   strcmp(ent->d_name, "dev")  == 0 || 
                                   strcmp(ent->d_name, "tmp")  == 0 ||
                                   strcmp(ent->d_name, "sys")  == 0 ||  
                                   strcmp(ent->d_name, "boot") == 0 ||
                                   strcmp(ent->d_name, "lib")  == 0 ||
                                   strcmp(ent->d_name, "opt")  == 0 ||
                                   strcmp(ent->d_name, "usr")  == 0 ||    
                                   strcmp(ent->d_name, "Application Data")  == 0
                                   )
                                        continue;	

                        #elif   _WIN32
                                if(strchr(ent->d_name, '$') != NULL)
                                        continue;
                                        
                        #endif

                        if(folder->result_length == folder->result_size)
                        {   
                                int old_size = folder->result_size;
                                folder->result_size += 128;
                                
                                char **template = malloc(folder->result_size * sizeof(char *));
                                memcpy(template, folder->result, old_size * sizeof(char *));
                                memset(folder->result, 0, old_size * sizeof(char *));
                                free(folder->result);

                                folder->result = template;

                                long *template2 = malloc(folder->result_size * sizeof(long));
                                memcpy(template2, folder->file_size, old_size * sizeof(long));
                                memset(folder->file_size, 0, old_size * sizeof(long));
                                free(folder->file_size);

                                folder->file_size = template2;
                        }

                        if(strstr(ent->d_name, folder->filename) != NULL)
                        {       
                                /* Set the necessary space for the dir */
                                folder->result[folder->result_length] = malloc(strlen(directory) + strlen(ent->d_name) + 1);
                                /* Assemble the dir string */
                                strcpy(folder->result[folder->result_length], directory);
                                strcat(folder->result[folder->result_length], ent->d_name);

                                FILE *f = fopen(folder->result[folder->result_length], "r");
                                
                                fseek(f, 0L, SEEK_END);
                                long len = ftell(f);

                                fclose(f);

                                folder->file_size[folder->result_length] = len;

                                /* Increase the dir index */
                                folder->result_length++;
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

/**
#################
Resent root dir
#################

* Set a new root directory into the struct
*/
void reset_directory(Folder* folder)
{
        #ifdef _WIN32
                folder->root_dir = realloc(folder->root_dir, strlen("C:\\") +1);
                folder->curr_dir = realloc(folder->curr_dir, strlen("C:\\") +1);

                strcpy(folder->root_dir, "C:\\");
                strcpy(folder->curr_dir, "C:\\");

        #elif __LINUX__
                folder->root_dir = realloc(folder->root_dir, strlen("/") +1);
                folder->curr_dir = realloc(folder->curr_dir, strlen("/") +1);

                strcpy(folder->root_dir, "/");
                strcpy(folder->curr_dir, "/");

        #endif
}

/**
#################
Clear
#################
*/
void clear(Folder* folder)
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
                for(int i=0; i < folder->result_length; i++)
                        printf("%s\n", folder->result[i]);
        else 
                /* If the array is empty return an error code */
                return 1;

        return 0;
}
