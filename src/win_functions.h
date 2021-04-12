#include <windows.h>

/*
#################
win_disk_get
#################

* Get all used drives in windows os
*/
void win_disks_get(Folder* folder)
{
    DWORD dword_size = MAX_PATH;
    char driver_array[MAX_PATH] = {0};
    DWORD dword_result = GetLogicalDriveStrings(dword_size, driver_array);

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
}


/*
#################
win_disk_search
#################

* Search the filename for each disk found
*/
void win_disks_search(Folder* folder)
{   
    if(folder->win_disks > 0)
    {
        for(int i=0; i < folder->win_disks_lenght; i++)
        {
            change_root_directory(folder, folder->win_disks[i]);
            find_file(folder);
        }
    }
}