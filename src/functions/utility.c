#include "../header/struct.c"
#include "../header/ftlib.h"


/**
#################
Set filter
#################

* Fill the filter array with user given data
* return EXIT_SUCCESS if the filter is set
* return EXIT_FAILURE if the filter array if full 
*/
int add_filter(Folder* folder, char* new_filter)
{   
	if(folder->filter_length >= FILTER_LIMIT)
		return EXIT_FAILURE;
	
	folder->filter_array[folder->filter_length] = malloc(strlen(new_filter) + 1);
	strcpy(folder->filter_array[folder->filter_length], new_filter);
	folder->filter_length++;

	return EXIT_SUCCESS;
}


/**
#################
Apply filter
#################

* Fill a given pointer array with filter matching results
* Return flt_result_array length
*/
int apply_filter(Folder* folder, char* flt_result_array[], long flt_size_array[], int flt_index)
{   
	for(int i=folder->result_index; i < folder->result_length; i++)
	{   
		for(int j=0; j < folder->filter_length; j++)
		{
			/* Check if the result contains the filter */
			if(strstr(folder->result_array[i], folder->filter_array[j]) != NULL)
			{   
				flt_result_array[flt_index] = malloc(strlen(folder->result_array[i]) + 1);
				strcpy(flt_result_array[flt_index], folder->result_array[i]);
				
				flt_size_array[flt_index] = folder->size_array[i];

				flt_index++;
				break;
			}
		}
	}
	folder->result_index = folder->result_length;

	return flt_index;
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
	free(folder->size_array);
	free(folder->separator);
	
	for(int i=0; i < folder->result_length; i++)
		free(folder->result_array[i]);
	free(folder->result_array);

	#ifdef _WIN32
		for(int i=0; i < folder->win_disk_length; i++)
			free(folder->win_disk_array[i]);
		free(folder->win_disk_array);
	#endif

	for(int i=0; i < folder->filter_length; i++)
		free(folder->filter_array[i]);
	free(folder->filter_array);
}


/**
#################
Print (debug)
#################
*/
void print(Folder* folder)
{
	for(int i=0; i < folder->result_length; i++)
	{
		printf("%s --- %ld\n", folder->result_array[i], folder->size_array[i]);
	}
}
