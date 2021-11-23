#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

/** Defining some constants for later use */

#define MAX_CHAR 30
#define INPUT_CHAR 15


/** Defining a function that takes one argument to list a specified directory */

int listing_directory(char* dirname)
{
  /** Selecting the specified directory through DIR struct */
  DIR* dir = opendir(dirname);

  /** returning string if directory is empty */
  if (dir == NULL)
  {
    printf("Empty directory");
  }

  /** defining directory entry structure type for reading */
  struct dirent* entity;
  entity = readdir(dir);

  /** Listing all files and directories inside the directory */
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





/** Searching for tags by listing directories recursively */

int find_tag(const char* directory, char* entered_tag)
{
  DIR *dir = opendir(directory);

  struct dirent* entity;
  entity = readdir(dir);


  while (entity)
  {
//    printf("%s/%s\n", directory, entity->d_name);
//    printf("ENTERED TAG: %s\n", entered_tag);

    if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0)
    {
      char tag_file[150];
      char path[100] = { 0 };
      strcat(path, directory);
      strcat(path, "/");
      strcat(path, entity->d_name);
      snprintf(tag_file, sizeof(tag_file), "%s/.pm_tag", path);

      if (access(tag_file, F_OK) == 0)
      {
//        printf("%s\n", tag_file);

        FILE *fp;
        char tag[20];

        fp = fopen(tag_file, "r");
        fseek(fp, 0, SEEK_END);
        long int len = ftell(fp);


        if(len > 0)
        {
          rewind(fp);
          fgets(tag, 20, fp);
          strtok(tag, "\n");
        }



        if (strcmp(entered_tag, tag) == 0)
        {
          printf("%s\n", path);
        }
        else
        {
          printf("Tag not found\n");
        }

        fclose(fp);
      }
      else
      {
        printf("There are no tags\n");
      }
      find_tag(path, entered_tag);

    }
    entity = readdir(dir);
    }

  closedir(dir);
}




/** Defining a function which looks in to the subfolders */

const char* slash ="/";
void make_path(char result[], char path[], char fname[])
{

  strcpy(result, path);
  strcat(result, slash);
  strcat(result, fname);

}
int list_file_type(char type, char result[100][100], int index, char path[])
{
  DIR *d;
  struct dirent *dir;
  d = opendir(path);
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
      {
        if (type == 1 && dir->d_type == DT_DIR)
          {
            if (dir->d_name[0] != '.')
              {
                char temp[100];
                make_path(temp, path, dir->d_name);
                strcpy(result[index++], temp);
                index = list_file_type(1, result, index, temp);
              }
          }
        if (type == 2 && dir->d_type == DT_REG)
          {
            strcpy(result[index++],dir->d_name);
          }
      }
      closedir(d);
  }
  return(index);
}



/** Defining a function which creates a new folder structure */
int creating_directory(char *newdir)
{
  int folder = mkdir(newdir, 0777);

  chdir(newdir);

  mkdir("bin", 0777);
  mkdir("docs", 0777);
  mkdir("lib", 0777);
  mkdir("src", 0777);
  mkdir("tests", 0777);

}



int main(int argc, char *argv[])
{
  /** Defining constants for options in program */
  char manage_dir[INPUT_CHAR] = "mngdir";
  char create_git[INPUT_CHAR] = "git";
  char input[INPUT_CHAR];
  char s[100];

  while (argc < 2 && strcmp(input, "quit") != 0)
  {
    printf("\n\n\nPROJECT MANAGER TOOL\n\n\n\n");

    printf("Select an option: \n\n");
    printf("\"mngdir\" - To manage directories\n");
    printf("\"git\"    - To init git\n");
    printf("\"quit\"   - To exit\n\n\n>>");

    /** User input for options */
    fgets(input, INPUT_CHAR, stdin);
    strtok(input, "\n");

    if (strcmp(input, "dir") == 0)
    {
      printf("%s\n", getcwd(s, 100));
    }

    if (strcmp(input, manage_dir) == 0)
    {
      /** Defining options for managing directories */
      char new_dir[MAX_CHAR];
      char option_input[INPUT_CHAR];
      char remove_dir[INPUT_CHAR] = "rmdir";
      char make_dir[INPUT_CHAR] = "mkdir";
      char rename_dir[INPUT_CHAR] = "rndir";
      char move_dir[INPUT_CHAR] = "mvdir";
      char back_dir[INPUT_CHAR] = "bcdir";


      /** Listing current directory */
      printf("\n\n\nCurrent directory:\n");
      listing_directory(".");

      /** Options for managing directories */
      printf("\nSelect an option: \n\n");
      printf("\"rmdir\" - To remove directory\n");
      printf("\"mkdir\" - To create directory\n");
      printf("\"rndir\" - To rename directory\n");
      printf("\"mvdir\" - To move to a directory\n");
      printf("\"bcdir\" - To move back directory\n\n");
      printf("\"back\"  - To go back to main menu\n\n\n>>");


      /** User input for managing directory options */
      fgets(option_input, INPUT_CHAR, stdin);
      strtok(option_input, "\n");

      if(strcmp(option_input, remove_dir) == 0)
      {
        printf("REMOVING DIR\n");


      }
      else if(strcmp(option_input, make_dir) == 0)
      {

        /** Taking an input for a new directory */
        printf("Enter the name of the new directory: ");
        fgets(new_dir, MAX_CHAR, stdin);

        /** Clarifying a raw string value by discarding the "\n" in the end */
        strtok(new_dir, "\n");

        /** Blocking clashing entries with current files */
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
      system("git init; git remote add origin https://csgitlab.ac.uk/nn020334/pc20_pm_cw2.git");
    }
  }



  if (strcmp(argv[1], "create_project") == 0)
  {
    if (access(argv[2], F_OK) == 0)
    {
      printf("A folder of that name already exists. Aborting.\n");
    }

    else if (argc > 3 || argc < 3 || strcmp(argv[2], "") == 0)
    {
      printf("Wrong number of parameters, should be pm command arg\n");
    }

    else if (strchr(argv[2], ' ') != 0)
    {
      printf("Bad characters in folder name\n");
    }

    else
    {
      creating_directory(argv[2]);
      system("git init; git remote add origin https://csgitlab.ac.uk/nn020334/pc20_pm_cw2.git");
    }
  }



  else if (strcmp(argv[1], "add_feature") == 0)
  {
    if (access(argv[2], F_OK) == 0)
    {
      printf("A folder of that name already exists. Aborting.\n");
    }

    else if (argc > 3 || argc < 3 || strcmp(argv[2], "") == 0)
    {
      printf("Wrong number of parameters, should be pm command arg\n");
    }

    else if (strchr(argv[2], ' ') != 0)
    {
      printf("Bad characters in folder name\n");
    }

    else
    {
      char new_branch[50];

      snprintf(new_branch, sizeof(new_branch), "git branch %s", argv[2]);

      system(new_branch);
      creating_directory(argv[2]);
    }

  }



  else if (strcmp(argv[1], "add_tag") == 0)
  {
    FILE *fp = NULL;

    char content[5];

    if(access(".pm_tag", F_OK) == 0)
    {
      fp = fopen(".pm_tag", "r");
      fgets(content, sizeof(content), fp);
      printf("Already a tag for this folder\n");
      printf("%s", content);

      fclose(fp);
      fp = NULL;
    }

    else if(argc > 3 || argc < 3 || strcmp(argv[2], "") == 0)
    {
      printf("Wrong number of parameters, should be pm command arg\n");
    }

    else if (strchr(argv[2], ' ') != 0)
    {
      printf("Bad characters in folder name\n");
    }

    else
    {
      fp = fopen(".pm_tag", "w+");

      snprintf(content, 50, "%s\n", argv[2]);

      fputs(content, fp);
      fclose(fp);
      fp = NULL;
    }
  }



  else if (strcmp(argv[1], "find_tag") == 0)
  {
    if (argc > 3 || argc < 3 || strcmp(argv[2], "") == 0)
    {
      printf("Wrong number of parameters, should be pm command arg\n");
    }

    else if (strchr(argv[2], ' ') != 0)
    {
      printf("Bad characters in folder name\n");
    }

    else
    {
      find_tag(".", argv[2]);
    }
  }



  else if (strcmp(argv[1], "move_by_tag") == 0)
  {

  }

  else if (strcmp(argv[1], "output_svg") == 0)
  {

  }

  else
  {
    printf("Wrong number of parameters, should be pm command arg\n");
  }

  return 0;

}
