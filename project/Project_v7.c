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

    else if(strcmp(user_input, create_git) == 0) /** Checking for command match */
    {
      system("git init; git remote add origin https://csgitlab.ac.uk/nn020334/pc20_pm_cw2.git"); /** Initializing git */
    }
  }



  if (strcmp(argv[1], "create_project") == 0) /** If command "create_project" - creating new project */
  {
    if (access(argv[2], F_OK) == 0) /** Checking for existing folders or files */
    {
      printf("A folder of that name already exists. Aborting.\n");
    }

    else if (argc > 3 || argc < 3 || strcmp(argv[2], "") == 0) /** Checking for unallowed number of arguments */
    {
      printf("Wrong number of parameters, should be pm command arg\n");
    }

    else if (strchr(argv[2], ' ') != 0) /** Checking for unallowed spaces in the 3rd argument */
    {
      printf("Bad characters in folder name\n");
    }

    else
    {
      creating_directory(argv[2]); /** Calling a function for creating a directory with folders: bin, docs, lib, src, tests */
      system("git init; git remote add origin https://csgitlab.ac.uk/nn020334/pc20_pm_cw2.git"); /** Initializing git */
    }
  }



  else if (strcmp(argv[1], "add_feature") == 0) /** If command "add_feature" - creating new feature */
  {
    if (access(argv[2], F_OK) == 0) /** Checking for existing folders or files */
    {
      printf("A folder of that name already exists. Aborting.\n");
    }

    else if (argc > 3 || argc < 3 || strcmp(argv[2], "") == 0) /** Checking for unallowed number of arguments */
    {
      printf("Wrong number of parameters, should be pm command arg\n");
    }

    else if (strchr(argv[2], ' ') != 0) /** Checking for unallowed spaces in the 3rd argument */
    {
      printf("Bad characters in folder name\n");
    }

    else
    {
      char new_branch[50];

      snprintf(new_branch, sizeof(new_branch), "git branch %s", argv[2]); /** Composing git command */

      system(new_branch); /** executing git */
      creating_directory(argv[2]); /** Calling a function for creating a directory with folders: bin, docs, lib, src, tests */
      printf("Feature successfully added!\n");
    }

  }



  else if (strcmp(argv[1], "add_tag") == 0) /** If command "add_tag" - adding a new tag */
  {
    FILE *fp = NULL; /** Establishing a pointer to the FILE structure */

    char content[15];

    if(access(".pm_tag", F_OK) == 0) /** Checking for existing folders or files */
    {
      fp = fopen(".pm_tag", "r"); /** Opening a file */
      fgets(content, sizeof(content), fp); /** Storing file's contents in a variable */
      printf("Already a tag for this folder\n");
      printf("%s", content);

      fclose(fp); /** Closing a file */
      fp = NULL;
    }

    else if(argc > 3 || argc < 3 || strcmp(argv[2], "") == 0) /** Checking for unallowed number of arguments */
    {
      printf("Wrong number of parameters, should be pm command arg\n");
    }

    else if (strchr(argv[2], ' ') != 0) /** Checking for unallowed spaces in the 3rd argument */
    {
      printf("Bad characters in folder name\n");
    }

    else
    {
      fp = fopen(".pm_tag", "w+"); /** Opening a file */

      snprintf(content, 10, "%s\n", argv[2]); /** Constructing a string */

      fputs(content, fp);  /** Writing to a file */
      fclose(fp); /** Closing a file */
      fp = NULL;
      printf("Tag successfully added!\n");
    }
  }



  else if (strcmp(argv[1], "find_tag") == 0) /** If command "find_tag" - finding a tag */
  {
    if (argc > 3 || argc < 3 || strcmp(argv[2], "") == 0) /** Checking for unallowed number of arguments */
    {
      printf("Wrong number of parameters, should be pm command arg\n");
    }

    else if (strchr(argv[2], ' ') != 0) /** Checking for unallowed spaces in the 3rd argument */
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

      if (access(".pm_tag", F_OK) == 0) /** Checking for existing folders or files */
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

  else if (strcmp(argv[1], "rename_directory") == 0) /** If command "rename_directory" - renaming existing directory */
  {
    if (argc > 4 || argc < 4 || strcmp(argv[2], "") == 0 || strcmp(argv[3], "") == 0) /** Checking for unallowed number of arguments */
    {
      printf("Wrong number of parameters, should be pm rename_directory existing_directory new_name\n");
    }

    else if (strchr(argv[2], ' ') != 0 || strchr(argv[3], ' ') != 0) /** Checking for unallowed spaces in the 3rd and 4th arguments */
    {
      printf("Bad characters in folder name\n");
    }

    else if (access(argv[2], F_OK) == 0) /** Checking for existing folders or files */
    {
      rename(argv[2], argv[3]);
      printf("Directory successfully renamed!\n");
    }

    else /** If directory not found - Poping an error message */
    {
      printf("Couldn't find directory: %s\n", argv[2]);
    }

  }

  else if (strcmp(argv[1], "move_by_tag") == 0) /** If command "move_by_tag" - creating new project */
  {
    if (argc > 4 || argc < 4 || strcmp(argv[2], "") == 0 || strcmp(argv[3], "") == 0) /** Checking for unallowed number of arguments */
    {
      printf("Wrong number of parameters, should be pm move_by_tag source_tag new_location_tag\n");
    }

    else if (strchr(argv[2], ' ') != 0 || strchr(argv[3], ' ') != 0) /** Checking for unallowed spaces in the 3rd and 4th arguments */
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


      if (access(".pm_tag", F_OK) == 0) /** Checking for existing folders or files */
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

  else if (strcmp(argv[1], "output_svg") == 0) /** If command "output_svg" - creating WBS tree diagram */
  {
    if (argc > 3 || argc < 3 || strcmp(argv[2], "") == 0) /** Checking for unallowed number of arguments */
    {
      printf("Wrong number of parameters, should be pm command directory\n");
    }

    else if (strchr(argv[2], ' ') != 0) /** Checking for unallowed spaces in the 3rd argument */
    {
      printf("Bad characters in folder name\n");
    }

    else if (access(argv[2], F_OK) == 0) /** Checking for existing folders or files */
    {
      char directory[40];
      char result_list[100][100];
      char *dir[100];
      char results[200];
      char constructed_str[400];
      char num_of_slashes = 0;
      char star[2] = "*";
      char new_str[200];
      char *token[100];
      char variable[150];
      char variable2[20];
      char message[300];
      int count = 0;
      int num;
      int i = 0;
      int j = 0;


      system("sudo apt install -y graphviz default-jre; mkdir -p .opt/plantuml; cd .opt/plantuml; sudo curl -JLO http://sourceforge.net/projects/plantuml/files/plantuml.jar/download;");

      FILE *fp = fopen("tree_diagram.txt", "w");

      snprintf(directory, sizeof(directory), "./%s", argv[2]);

      int length = strlen(directory);
      char input[40];
      char cut_result[50];
      char cut_trimmed_result[40];
      char cut_str[40];

      for(int i = 0; i < length; i++)
      {
        input[i] = directory[i];
      }

      memset(cut_str, '\0', sizeof(cut_str));
      strncpy(cut_str, input, length);


      for (int i=0; i<list_file_type(1, result_list, 0, "."); i++)
      {
        snprintf(results, sizeof(results), "%s", result_list[i]); /** path to the directory */

        for(int i = 0; i < length; i++)
        {
          cut_result[i] = results[i];
        }

        memset(cut_trimmed_result, '\0', sizeof(cut_trimmed_result));
        strncpy(cut_trimmed_result, cut_result, length);


        if (strcmp(cut_trimmed_result, cut_str) == 0) /** if input path contains part of input execute code */
        {

          count = 0;
          j = 0;
          num_of_slashes = 0;
          strcpy(variable2, "");

          while (results[count] != '\0')
          {
            if (results[count] == '/')
            {
              num_of_slashes++; /** level of directory depth */
            }
            count++;
          }

          strcpy(new_str, results);
          token[j] = strtok(new_str, "/");

          while (token[j] != NULL)
          {
            token[++j] = strtok(NULL, "/");
          }
          snprintf(variable, sizeof(variable), "%s", token[j-1]);


          for (num = 0; num < num_of_slashes; num++)
          {
            strcat(variable2, "*");
          }
          if (strcmp(variable, "bin") != 0 && strcmp(variable, "docs") != 0 && strcmp(variable, "lib") != 0 && strcmp(variable, "src") != 0 && strcmp(variable, "tests") != 0) /** Excluding bin, docs, lib, src, tests */
          {
            snprintf(message, sizeof(message)+50, "%s %s\n", variable2, variable);
            strcat(constructed_str, message);
          }
        }
      }

      char mess[500];
      char start_temp[20] = "@startwbs";
      char end_temp[20] = "@endwbs";
      int k = 0;


      while (constructed_str[k] != '*')
      {
        memmove(constructed_str, constructed_str+1, strlen(constructed_str));
      }

      snprintf(mess, 500, "%s\n%s%s\n", start_temp, constructed_str, end_temp);

      fputs(mess, fp);
      fclose(fp);

      system("java -jar .opt/plantuml/plantuml.jar tree_diagram.txt");
      printf("WBS Tree diagram output successful!");
    }

    else
    {
      printf("Couldn't find directory: %s\n", argv[2]);
    }
  }

  else if (strcmp(argv[2], "calculate_workload") == 0)
  {
    printf("Feature\n");
  }

  else
  {
    printf("Wrong command or number of parameters, should be pm command arg\n");
  }

  return 0;

}
