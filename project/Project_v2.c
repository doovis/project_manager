#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

/* Defining some constants for later use */

#define MAX_CHAR 30
#define INPUT_CHAR 15


/* This function takes one argument to list a specified directory */

int listing_directory(const char* dirname)
{
  /* Selecting the specified directory through DIR struct */
  DIR* dir = opendir(dirname);

  /* returning string if directory is empty */
  if (dir == NULL)
  {
    printf("Empty directory");
  }

  /* defining directory entry structure type for reading */
  struct dirent* entity;
  entity = readdir(dir);

  /* Listing all files and directories inside the directory */
  printf("_________\n");
  while (entity != NULL)
  {
    printf("%s\n", entity->d_name);
    entity = readdir(dir);
  }
  printf("_________\n\n\n");

  closedir(dir);

  return 0;
}




int main(void)
{
  /* Defining constants for options in program */
  char manage_dir[INPUT_CHAR] = "mngdir";
  char create_git[INPUT_CHAR] = "git";
  char input[INPUT_CHAR];

  printf("PROJECT MANAGER TOOL\n\n\n\n");
  while (strcmp(input, "quit") != 0)
  {
    printf("Select an option: \n\n");
    printf("\"mngdir\" - To manage directories\n");
    printf("\"git\"    - To init git\n");
    printf("\"quit\"   - To exit\n\n\n>>");

    /* User input for options */
    fgets(input, INPUT_CHAR, stdin);
    strtok(input, "\n");

    if (strcmp(input, manage_dir) == 0)
    {
      /* Defining options for managing directories */
      char new_dir[MAX_CHAR];
      char option_input[INPUT_CHAR];
      char remove_dir[INPUT_CHAR] = "rmdir";
      char make_dir[INPUT_CHAR] = "mkdir";
      char rename_dir[INPUT_CHAR] = "rndir";
      char move_dir[INPUT_CHAR] = "mvdir";
      char back_dir[INPUT_CHAR] = "bcdir";


      /* Listing current directory */
      printf("\n\n\nCurrent directory:\n");
      listing_directory(".");

      /* Options for managing directories */
      printf("\nSelect an option: \n\n");
      printf("\"rmdir\" - To remove directory\n");
      printf("\"mkdir\" - To create directory\n");
      printf("\"rndir\" - To rename directory\n");
      printf("\"mvdir\" - To move to a directory\n");
      printf("\"bcdir\" - To move back directory\n\n");
      printf("\"back\"  - To go back to main menu\n\n\n>>");


      /* User input for managing directory options */
      fgets(option_input, INPUT_CHAR, stdin);
      strtok(option_input, "\n");

      if(strcmp(option_input, remove_dir) == 0)
      {
        printf("REMOVING DIR\n");


      }
      else if(strcmp(option_input, make_dir) == 0)
      {

        /* Taking an input for a new directory */
        printf("Enter the name of the new directory: ");
        fgets(new_dir, MAX_CHAR, stdin);

        /* Clarifying a raw string value by discarding the "\n" in the end */
        strtok(new_dir, "\n");

        /* Blocking clashing entries with current files */
        while (access(new_dir, F_OK) == 0)
        {
          printf("\n\nExisting file - try different name: ");
          fgets(new_dir, MAX_CHAR, stdin);
          strtok(new_dir, "\n");
        }


        printf("\n\nCreating directory: %s...\n", new_dir);
        int folder = mkdir(new_dir, 0777);

        printf("%s has been successfully created\n\n\n", new_dir);
        printf("new directory\n\n");
        listing_directory(".");

      }

      else if(strcmp(option_input, rename_dir) == 0)
      {

      }

      else if(strcmp(option_input, move_dir) == 0)
      {

      }

      else if(strcmp(option_input, back_dir) == 0)
      {

      }

      else
      {
        continue;
      }

    }

    else if(strcmp(input, create_git) == 0)
    {
      FILE *fp;

      fp = fopen("Makefile", "w+");

      fputs("git_init:\n	git init ;\\\n	git add -A ;\\\n	git remote add https://csgitlab.reading.ac.uk/nn020334/pc20_pm_cw2.git ;\\\n	git push -u origin master", fp);

      system("./ ; make");
/*
      execl("Makefile", "Makefile", NULL);
*/

    }

  }

  return 0;

}
