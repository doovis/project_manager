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
/*
void find_tag(const char* directory, char* entered_tag)
{
  DIR *dir = opendir(directory);

  struct dirent* entity;
  entity = readdir(dir);


  while (entity)
  {
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

        printf("%s", tag);

        if (strcmp(entered_tag, tag) == 0)
        {
          printf("%s\n", path);
        }


        fclose(fp);
      }
      find_tag(path, entered_tag);

    }
    entity = readdir(dir);
    }

  closedir(dir);
}
*/



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
  /** Defining constants for later use in program */
  char manage_dir[INPUT_CHAR] = "mngdir";
  char create_git[INPUT_CHAR] = "git";
  char user_input[INPUT_CHAR];
  char cwd[100];

  /** Running project manager program in a loop */
  while (argc < 2 && strcmp(user_input, "quit") != 0)
  {
    system("clear");
    printf("PROJECT MANAGER TOOL\n\n\n\n");
    printf("Select an option: \n\n");
    printf("\"mngdir\" - To manage directories\n");
    printf("\"dir\"    - To print current directory\n");
    printf("\"git\"    - To init git\n");
    printf("\"quit\"   - To exit\n\n\n>>");

    /** User input for options */
    fgets(user_input, INPUT_CHAR, stdin);
    strtok(user_input, "\n");

    if (strcmp(user_input, "dir") == 0)
    {
      printf("%s\n", getcwd(cwd, sizeof(cwd)));
    }

    if (strcmp(user_input, manage_dir) == 0)
    {
      /** Defining options for managing directories */
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
        char dir_input[MAX_CHAR];
        printf("Enter the name of the directory you'd like to remove: ");
        fgets(dir_input, MAX_CHAR, stdin);
        strtok(dir_input, "\n");

        if (access(dir_input, F_OK) == 0)
        {
          rmdir(dir_input);
          printf("REMOVING DIR %s\n", dir_input);
        }
        else
        {
          printf("No such directory\n");
        }
      }
      else if(strcmp(option_input, make_dir) == 0)
      {
        char new_dir[MAX_CHAR];

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

        int folder = mkdir(new_dir, 0777);
        if (access(new_dir, F_OK) == 0)
        {
          printf("%s has been successfully created\n\n\n", new_dir);
          printf("new directory\n\n");
          listing_directory(".");
        }
        else
        {
          printf("Something went wrong\n");
        }
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
      }

    }

    else if(strcmp(user_input, create_git) == 0)
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
      printf("Feature successfully added!\n");
    }

  }



  else if (strcmp(argv[1], "add_tag") == 0)
  {
    FILE *fp = NULL;

    char content[10];

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

      snprintf(content, 10, "%s\n", argv[2]);

      fputs(content, fp);
      fclose(fp);
      fp = NULL;
      printf("Tag successfully added!\n");
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
      char result_list[100][100];
      char tag_name[20];
      char path[200];
      int local_tag = 0;
      int found = 0;

      if (access(".pm_tag", F_OK) == 0)
      {
        FILE *fp;
        char tag[20];

        fp = fopen(".pm_tag", "r");
        fseek(fp, 0, SEEK_END);
        long int len = ftell(fp);

        if(len > 0)
        {
          rewind(fp);
          fgets(tag, 20, fp);
          strtok(tag, "\n");
        }

        if (strcmp(argv[2], tag) == 0)
        {
          local_tag++;
          printf("%s\n", "./");
        }
        fclose(fp);
      }

      if (local_tag < 1)
      {
        for (int i=0; i<list_file_type(1, result_list, 0, "."); i++)
        {
          snprintf(path, sizeof(path), "%s/.pm_tag", result_list[i]);

          if (access(path, F_OK) == 0)
          {
            FILE *fp;
            char tag[20];

            fp = fopen(path, "r");
            fseek(fp, 0, SEEK_END);
            long int len = ftell(fp);

            if(len > 0)
            {
              rewind(fp);
              fgets(tag, 20, fp);
              strtok(tag, "\n");
            }

            if (strcmp(argv[2], tag) == 0)
            {
              found = 1;
              printf("%s\n", result_list[i]);
            }
            fclose(fp);
          }
        }
        if (found == 0)
        {
          printf("Tag not found\n");
        }
      }
    }
  }

  else if (strcmp(argv[1], "rename_directory") == 0)
  {
    if (argc > 4 || argc < 4 || strcmp(argv[2], "") == 0 || strcmp(argv[3], "") == 0)
    {
      printf("Wrong number of parameters, should be pm rename_directory existing_directory new_name\n");
    }

    else if (strchr(argv[2], ' ') != 0 || strchr(argv[3], ' ') != 0)
    {
      printf("Bad characters in folder name\n");
    }

    else if (access(argv[2], F_OK) == 0)
    {
      rename(argv[2], argv[3]);
      printf("Directory successfully renamed!\n");
    }

    else
    {
      printf("Couldn't find directory: %s\n", argv[2]);
    }

  }

  else if (strcmp(argv[1], "move_by_tag") == 0)
  {
    if (argc > 4 || argc < 4 || strcmp(argv[2], "") == 0 || strcmp(argv[3], "") == 0)
    {
      printf("Wrong number of parameters, should be pm move_by_tag source_tag new_location_tag\n");
    }

    else if (strchr(argv[2], ' ') != 0 || strchr(argv[3], ' ') != 0)
    {
      printf("Bad characters in folder name\n");
    }

    else
    {
      char result_list[100][100];
      char path[200];
      char arg2_tag[100];
      char arg3_tag[100];
      int found = 0;
      int cur_dir = 0;


      if (access(".pm_tag", F_OK) == 0)
      {
        FILE *fp;
        char tag[20];

        fp = fopen(".pm_tag", "r");
        fseek(fp, 0, SEEK_END);
        long int len = ftell(fp);

        if(len > 0)
        {
          rewind(fp);
          fgets(tag, 20, fp);
          strtok(tag, "\n");
        }

        if (strcmp(argv[2], tag) == 0)
        {
          cur_dir = 1;
          found += 2;
          printf("Cannot move current directory elsewhere\n");
//          snprintf(arg2_tag, sizeof(arg2_tag), "./");
        }

        else if (strcmp(argv[3], tag) == 0)
        {
          found++;
          snprintf(arg3_tag, sizeof(arg3_tag), "./");
        }
        fclose(fp);
      }
      if (cur_dir == 0)
      {
        for (int i=0; i<list_file_type(1, result_list, 0, "."); i++)
        {
          snprintf(path, sizeof(path), "%s/.pm_tag", result_list[i]);
          if (access(path, F_OK) == 0)
          {
            FILE *fp;
            char tag[20];

            fp = fopen(path, "r");
            fseek(fp, 0, SEEK_END);
            long int len = ftell(fp);


            if(len > 0)
            {
              rewind(fp);
              fgets(tag, 20, fp);
              strtok(tag, "\n");
            }

            if (strcmp(argv[2], tag) == 0)
            {
              found += 2;
              snprintf(arg2_tag, sizeof(arg2_tag), "%s", result_list[i]);
            }

            else if (strcmp(argv[3], tag) == 0)
            {
              found++;
              snprintf(arg3_tag, sizeof(arg3_tag), "%s", result_list[i]);
            }
            fclose(fp);
          }
        }

        if (found == 0)
        {
          printf("Tags not found\n");
        }

        if (found == 1)
        {
          printf("First tag not found\n");
        }

        if (found == 2)
        {
          printf("Second tag not found\n");
        }

        if (found == 3)
        {
          char *directory_str[30];
          char dir_path[120];
          char arg2[50];
          strcpy(arg2, arg2_tag);
          int i = 0;


          directory_str[i] = strtok(arg2_tag, "/");
          while (directory_str[i] != NULL)
          {
            directory_str[++i] = strtok(NULL, "/");
          }

          snprintf(dir_path, sizeof(dir_path), "%s/%s", arg3_tag, directory_str[i-1]);
          rename(arg2, dir_path);
          printf("Directory successfully moved!\n");
        }
      }
    }
  }

  else if (strcmp(argv[1], "output_svg") == 0)
  {
    char directories[100];
    char result_list[100][100];
    char *dir[100];
    char results[100];
    int count = 0;
/*
    system("sudo apt install -y graphviz default-jre; mkdir -p .opt/plantuml; cd .opt/plantuml; sudo curl -JLO http://sourceforge.net/projects/plantuml/files/plantuml.jar/download;");
    FILE *fp = fopen("tree_diagram.txt", "w");
    if(access(argv[2], F_OK) == 0)
*/
    for (int i=0; i<list_file_type(1, result_list, 0, "."); i++)
    {
      snprintf(results, sizeof(results)+10, "%s", result_list[i]);
//      printf("%s\n", result_list[i]);
//      results = strtok(directories, "/");
      while (results[count] != '\0')
      {
        if (results[count] == '/')
        {
          results[count] = '*';
        }
      count++;
      }
//      snprintf(results, sizeof(results), "%s", dir[i]);
      printf("%s\n", results);
    }
/*      if (access(path, F_OK) == 0)
      {

      }
*/










/*

    char string_of_dirs[100];
    char message[100];
    snprintf(message, sizeof(message), "%s", string_of_dirs);

    fputs(message, fp);
    fclose(fp);

    system("java -jar .opt/plantuml.jar tree_diagram.txt");
*/
  }

  else
  {
    printf("Wrong number of parameters, should be pm command arg\n");
  }

  return 0;

}
