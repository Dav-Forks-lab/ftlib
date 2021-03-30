#include <stdio.h>
#include <stdlib.h>
#include "ftlib.c"

int main()
{
  Folder* folder = malloc(sizeof(* folder));
  char filename[128];
  printf("Insert filename: ");
  scanf("%s", &filename);

  init(folder, filename);

  find_file(folder);
  
  puts("\nFINISHED!\n");

  print(folder);

  free(folder);

  return 0;
}
