#include "../header/struct.c"
#include "../header/ftlib.h"


/**
#################
Init
#################

* Initialize struct's data
*/
Folder* init() 
{      
	Folder* folder = malloc(sizeof(Folder));

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
		folder->win_disk_array= malloc(sizeof(char*) * 32);
		folder->win_disk_length = 0;

		if (dword_result > 0 && dword_result <= MAX_PATH)
		{
			char* drive = driver_array;
			while(*drive)
			{       
				folder->win_disk_array[folder->win_disk_length] = malloc(strlen(drive) + 1);
				strcpy(folder->win_disk_array[folder->win_disk_length], drive);

				// Point to the next drive
				drive += strlen(drive) + 1;
				folder->win_disk_length++;
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
	folder->result_fill_index = 128;
	folder->result_length = 0;
	folder->filter_length = 0;
	folder->result_index = 0;

	folder->filter_array = malloc(FILTER_LIMIT * sizeof(char *));
	folder->result_array = malloc(folder->result_fill_index * sizeof(char *));
	folder->size_array = malloc(folder->result_fill_index * sizeof(long));
	folder->curr_dir = malloc(strlen(folder->root_dir) + 1);
	folder->filename = NULL;
	
	strcpy(folder->curr_dir, folder->root_dir);

	return folder;
}
