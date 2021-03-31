/*
    INTESTAZIONE
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define FILTER_LIMIT 32

typedef struct {
    char *filename;
    char root_dir[4];
    char *curr_dir;
    char *ptr;
    char **result;
    char separator[1];
    char *filters[FILTER_LIMIT];
    long int result_lenght, filter_lenght, result_size;
} Folder;  


void init(Folder *folder, const char* filename) 
{      
    // Init sizes
    folder->result_size = 1024;
    folder->result_lenght = 0;
    folder->filter_lenght = 0;

    // Init filter's arrays
    memset(folder->filters, 0, sizeof(folder->filters));
    // Init result's array
    folder->result = malloc(folder->result_size * sizeof(char *));
    // Init dirs' array
    memset(folder->root_dir, '\0', sizeof(folder->root_dir));
    folder->filename = malloc(strlen(filename) + 1);
    strcpy(folder->filename, filename);    

    // Initialize directory based on the OS
    #ifdef _WIN32
        strcat(folder->root_dir, "C:\\");
        strcat(folder->separator, "\\");
    #elif __linux__ || __APPLE__
        strcat(folder->root_dir, "/");
        strcat(folder->separator, "/");
    #endif       

    // Set current directory
    folder->curr_dir = malloc(strlen(folder->root_dir));
    strcpy(folder->curr_dir, folder->root_dir);
}

void find_file(Folder *folder)
{   
    char* directory = malloc(strlen(folder->curr_dir) + 1);
    strcpy(directory, folder->curr_dir);

    DIR *dir;
    struct dirent *ent;

    if(strchr(directory, '.') != NULL)
        return;
         
    // Try to open the directory
    if((dir = opendir(directory)) != NULL)
    {   
        // Search in every sub-folder
        while((ent = readdir(dir)) != NULL)
        {  
	        // dev/fd folder not needed for the file searching	
            #ifdef __linux__
	        if(strstr(ent->d_name, "fd") != NULL || strstr(ent->d_name, "proc") != NULL)
		    continue;	
	    #endif
	    /* 
            If the directory points to the same o the prev
            directory the function retrun NULL
            */
            if(ent->d_name[strlen(ent->d_name) - 1] == '.' || ent->d_name[0] == '$')
                continue;

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
                // Set the necessary space for the dir
                folder->result[folder->result_lenght] = malloc(strlen(directory) + strlen(ent->d_name) +1);
                // Assemble the dir string
                strcpy(folder->result[folder->result_lenght], directory);
                strcat(folder->result[folder->result_lenght], ent->d_name);
                // Increase the dir index
                folder->result_lenght++;
            }

            // Set current directory
            folder->curr_dir = realloc(folder->curr_dir, strlen(directory) + strlen(ent->d_name) + strlen(folder->separator) + 2);
            strcpy(folder->curr_dir, directory);
            strcat(folder->curr_dir, ent->d_name);
            strcat(folder->curr_dir, folder->separator);
            // Start the search again with recursion
            find_file(folder);
        }
    }
    
    // Clear pointer data
    free(dir);
    free(ent);
}


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
    }
}

void apply_filter(Folder *folder, char* filtered_result[], int* index)
{   
    for(int i=0; i < folder->result_lenght; i++)
    {   
        for(int j=0; j < folder->filter_lenght; j++)
        {
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

void change_filename(Folder* folder, char* new_filename)
{
    folder->filename = realloc(folder->filename, strlen(new_filename) + 1);
    strcpy(folder->filename, new_filename);
}

// Print the result array
int print(Folder* folder)
{   
    // Check if the array is non empty
    if(folder->result[0])
        // Print array data
        for(int i=0; i < folder->result_lenght; i++)
            printf("%s\n", folder->result[i]);
    else 
        // If the array is empty return an error code
        return 1;

    return 0;
}



