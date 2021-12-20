#include "../include/ftlib.h"
#include "../ftl/ftl.h"
#include "../json_functions/fileMan.cpp"

void scan(char* directory)
{
	DIR* dir;
	struct dirent* ent;

	if(strchr(directory, '.') != NULL)
		return;

	if((dir = opendir(directory)) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{	
			char* tmp = (char*)malloc(strlen(ent->d_name));
			strcpy(tmp, ent->d_name);

			DFILE* file = f_init();
			f_setname(file, tmp);
			f_setpath(file, directory);
			//f_settype(file, ftype(tmp));
			//f_setsize(file, fsize(tmp));

			writefile(file);

			free(tmp);
		}
	}
}


