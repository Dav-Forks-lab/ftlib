#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <string.h>

#include "../src/header/ftlib.h"

/*
#################
Find file
#################

* This file is used to try functionalities and optimizations on windows
* As it's used for testing it's definitely not the best program you can get from the library 
*/

int print_data_on = 1;
/*
void print_data(Folder* folder)
{
	puts("Thread 2 started");
	long int old = 0;

	while(folder->result_index != folder->result_length || print_data_on)
	{       
		if(old != 0 && strcmp(folder->curr_dir, "/") == 0)
			continue;
		if(old != folder->result_length)
		{       
			for(int i=old; i < folder->result_length; i++)
			{
			    printf("\n--> %s %ld", folder->result_array[i], folder->size_array[i]);
			}
			old = folder->result_length;
		}
	}
}
*/
int main()
{
	pthread_t thread1, thread2;
	char* fName = malloc(1024);
	char* filter = malloc(1024);

	Folder* f = init();

	printf("\nInserisci il nome del file: ");
	scanf("%s", fName);

	set_filename(f, fName);

	clock_t start = clock();
	puts("Clock started");

	pthread_create(&thread1, NULL, (void*)search, (void*)f);
	pthread_join(thread1, NULL);
	
	print_data_on = 0;
	
	clock_t end = clock();
	
	puts("\n");
	
	printf("Time %f\n", (double)(end - start) / CLOCKS_PER_SEC);

	print(f);


	reset(f);
	free(f);


	return EXIT_SUCCESS;
}
