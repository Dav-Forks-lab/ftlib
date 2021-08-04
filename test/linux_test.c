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

void print_data(Folder* folder)
{
        puts("Thread 2 started");
        long int old = 0;
        long int idx = 0;
        char* array[4096];
        long size[4096];
        while(folder->result_index != folder->result_length || print_data_on)
        {       
                if(old != 0 && strcmp(folder->curr_dir, "/") == 0)
                        continue;
                if(old != folder->result_length)
                {       
                        idx = apply_filter(folder, array, size, idx);
                        for(int i=old; i < idx; i++)
                        {
                            printf("\n--> %s %ld", array[i], size[i]);
                        }
                        old = idx;
                }
        }
}

int main()
{
        Folder *f = malloc(sizeof(Folder));
        pthread_t thread1, thread2;
        char* fName = malloc(1024);
        char* filter = malloc(1024);

        printf("\nInserisci il nome del file: ");
        scanf("%s", fName);

        init(f, fName);

        printf("\nInserisci il filtro: ");
        scanf("%s", filter);

        add_filter(f, filter);

        pthread_create(&thread2, NULL, (void*)print_data, (void*)f);
    
        pthread_create(&thread1, NULL, (void*)find_file, (void*)f);    
        pthread_join(thread1, NULL);

        print_data_on = 0;

        pthread_join(thread2, NULL);

        puts("\nFINISHED\n");

        //print(f);

        reset(f);
        free(f);


        return EXIT_SUCCESS;
}