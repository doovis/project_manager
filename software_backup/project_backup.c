#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

/* Defining some constants for later use */

#define MAX_CHAR 30



/* This function takes one argument to list a specified directory */

const char* listing_directory(const char* dirname)
{
  /* Selecting the specified directory through DIR struct */
  DIR* d = opendir(dirname);

  /* returning void if directory is empty */
  if (d == NULL)
  {
    return "N/A";
  }

  /* defining directory entry structure type for reading */
  struct dirent* entity;
  entity = readdir(d);

  while (entity != NULL)
  {
      printf("%s\n", entity->d_name);
      entity = readdir(d);
  }

  closedir(d);

  return entity->d_name;
}




int main(void)
{
  char parent_directory[MAX_CHAR];
  printf("Enter the name of the parent directory: ");
  scanf("%[^\n]%*c", parent_directory);

  while (parent_directory == 1)
  {
    printf("directory %c already exists, try different name: ", *parent_directory);
    scanf("%[^\n]%*c", parent_directory);
  }
  int folder = mkdir("test", 0777);
  printf("%s has been successfully created\n", parent_directory);


  listing_directory(".");


return 0;

}
