#include "../header/ftlib.h"


/**
#################
Find file
#################

* Recursive search through file directories 
*/
void find_file(Folder* folder)
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
                                /* Folder not needed for the file searching */ 	
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
                        

                        if(folder->result_length == folder->result_fill_index)
                        {   
                                int old_size = folder->result_fill_index;
                                folder->result_fill_index += 128;
                                
                                char **template = malloc(folder->result_fill_index * sizeof(char *));
                                memcpy(template, folder->result_array, old_size * sizeof(char *));
                                memset(folder->result_array, 0, old_size * sizeof(char *));
                                free(folder->result_array);

                                folder->result_array = template;

                                long *template2 = malloc(folder->result_fill_index * sizeof(long));
                                memcpy(template2, folder->file_size_array, old_size * sizeof(long));
                                memset(folder->file_size_array, 0, old_size * sizeof(long));
                                free(folder->file_size_array);

                                folder->file_size_array = template2;
                        }

                        if(strstr(ent->d_name, folder->filename) != NULL)
                        {       
                                /* Set the necessary space for the dir */
                                folder->result_array[folder->result_length] = malloc(strlen(directory) + strlen(ent->d_name) + 1);
                                /* Assemble the dir string */
                                strcpy(folder->result_array[folder->result_length], directory);
                                strcat(folder->result_array[folder->result_length], ent->d_name);

                                FILE *f = fopen(folder->result_array[folder->result_length], "r");
                                
                                fseek(f, 0L, SEEK_END);
                                long len = ftell(f);

                                fclose(f);

                                folder->file_size_array[folder->result_length] = len;

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
