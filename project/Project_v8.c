#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>


/** Defining macros */
#define MAX_CHAR 50
#define INPUT_CHAR 30



/** Defining a function that lists specified directory */
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

  /** Listing all directories inside the directory */
  printf("__________________\n");
  while ((entity = readdir(dir)) != NULL)
  {
    if (entity->d_type == DT_DIR)
    {
      if (entity->d_name[0] != '.')
      {
        printf("%s\n", entity->d_name);
      }
    }
  }
  printf("__________________\n\n");

  closedir(dir);

  return 0;
}


/** Defining a path construction function */
const char* slash = "/";
void make_path(char result[], char path[], char fname[])
{

  strcpy(result, path);
  strcat(result, slash);
  strcat(result, fname);

}


/** Defining recursive file directory search function */
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
  /** If only 1 argument supplied enter wizard version */
  if (argc == 1)
  {
    char user_input[INPUT_CHAR];

    /** Running program untill quit argument given */
    while (strcmp(user_input, "quit") != 0)
    {
      char manage_dir[INPUT_CHAR] = "mngdir";
      char create_proj[INPUT_CHAR] = "create_project";
      char create_git[INPUT_CHAR] = "git";
      char delaying_input[INPUT_CHAR];
      char cwd[100];

      system("clear");

      printf("PROJECT MANAGER TOOL\n\n");
      printf("PATH:\n%s:\n\n\n\n", getcwd(cwd, sizeof(cwd)));
      printf("SELECT AN OPTION: \n\n");
      printf("\"mngdir\"         - To manage directories\n");
      printf("\"create_project\" - To create default project structure\n");
      printf("\"git\"            - To init git\n");
      printf("\"quit\"           - To exit\n\n\n>>");


      /** User input for options */
      fgets(user_input, INPUT_CHAR, stdin);
      strtok(user_input, "\n");

      /** Entering directory manager option if appropriate string supplied */
      if (strcmp(user_input, manage_dir) == 0)
      {
        /** Defining options for managing directories */
        char remove_dir[INPUT_CHAR] = "rmdir";
        char make_dir[INPUT_CHAR] = "mkdir";
        char rename_dir[INPUT_CHAR] = "rndir";
        char move_dir[INPUT_CHAR] = "mvdir";
        char up_dir[INPUT_CHAR] = "updir";
        char down_dir[INPUT_CHAR] = "dwdir";
        char option_input[INPUT_CHAR];

        strcpy(option_input, "");

        /** Looping in directory manager option until appropriate string given */
        while (strcmp(option_input, "back") != 0)
        {
          system("clear");

          /** Listing current directory */
          printf("%s:\n", getcwd(cwd, sizeof(cwd)));
          listing_directory(".");

          /** Options for managing directories */
          printf("\nSELECT AN OPTION: \n\n");
          printf("\"rmdir\"      - To remove directory\n");
          printf("\"mkdir\"      - To create directory\n");
          printf("\"rndir\"      - To rename directory\n");
          printf("\"mvdir\"      - To move directory\n");
          printf("\"updir\"      - To move up a directory\n");
          printf("\"dwdir\"      - To move down directory\n\n");
          printf("\"output_svg\" - To graph the directory\n");
          printf("\"back\"       - To go back to main menu\n\n>>");


          /** User input for managing directory options */
          fgets(option_input, INPUT_CHAR, stdin);
          strtok(option_input, "\n");

          /** Removing directory option */
          if(strcmp(option_input, remove_dir) == 0)
          {
            char dir_input[MAX_CHAR];
            printf("Name of directory to remove: ");
            fgets(dir_input, MAX_CHAR, stdin);
            strtok(dir_input, "\n");

            /** Checking if input name exists */
            if (access(dir_input, F_OK) == 0)
            {
              char buffer[60];
              snprintf(buffer, sizeof(buffer), "rm -rf %s", dir_input);
              system(buffer);
            }
            else
            {
              printf("No such directory\n");
              printf("Press enter...");
              fgets(delaying_input, 2, stdin);
            }
          }

          /** Creating directory option */
          else if(strcmp(option_input, make_dir) == 0)
          {
            char new_dir[MAX_CHAR];
            int err_flag = 1;

            /** Taking an input for a new directory */
            printf("Name of new directory: ");
            fgets(new_dir, MAX_CHAR, stdin);

            /** Clarifying a raw string value by discarding the "\n" in the end */
            strtok(new_dir, "\n");

            /** If flag is activated - run in a loop */
            while (err_flag == 1)
            {
              err_flag = 0;

              /** Checking for empty entries */
              if (strchr(new_dir, ' ') != 0 || strcmp(new_dir, "\n") == 0)
              {
                err_flag = 1;
                printf("Invalid entry. Try again...\n");
                printf("Name of new directory: ");
                fgets(new_dir, MAX_CHAR, stdin);
                strtok(new_dir, "\n");
              }


              else
              {
                /** Blocking clashing entries with current files */
                while (access(new_dir, F_OK) == 0)
                {
                  printf("Existing file - try different name: ");
                  fgets(new_dir, MAX_CHAR, stdin);
                  strtok(new_dir, "\n");
                }

                int folder = mkdir(new_dir, 0777);
              }
            }
          }

          /** Renaming directory option */
          else if(strcmp(option_input, rename_dir) == 0)
          {
            char dir_to_rn[50];
            char new_name[50];

            /** Storing input */
            printf("Directory to rename: ");
            fgets(dir_to_rn, MAX_CHAR, stdin);
            strtok(dir_to_rn, "\n");

            /** Checking for existing directory */
            if (access(dir_to_rn, F_OK) == 0)
            {
              printf("New name: ");
              fgets(new_name, MAX_CHAR, stdin);
              strtok(new_name, "\n");

              /** Averting empty entries */
              if (strchr(new_name, ' ') != 0 || strcmp(new_name, "\n") == 0)
              {
                printf("Invalid entry\n");
                printf("Press enter...");
                fgets(delaying_input, 2, stdin);
              }

              else
              {
                rename(dir_to_rn, new_name);
              }
            }

            /** Averting empty entries */
            else
            {
              printf("Directory doesn't exist\n");
              printf("Press enter...");
              fgets(delaying_input, 2, stdin);
            }
          }

          /** Moving directory option */
          else if(strcmp(option_input, move_dir) == 0)
          {
            char dir_to_move[40];
            char destination[40];
            char buffer[80];

            /** Storing input */
            printf("Directory to move: ");
            fgets(dir_to_move, MAX_CHAR, stdin);
            strtok(dir_to_move, "\n");

            /** Checking for input directory existance */
            if (access(dir_to_move, F_OK) == 0)
            {
              printf("Destination: ");
              fgets(destination, MAX_CHAR, stdin);
              strtok(destination, "\n");

              /** Checking for input directory existance */
              if (access(destination, F_OK) == 0)
              {
                snprintf(buffer, sizeof(buffer), "%s/%s", destination, dir_to_move);
                rename(dir_to_move, buffer);
              }

              else
              {
                printf("Directory doesn't exist\n");
                printf("Press enter...");
                fgets(delaying_input, 2, stdin);
              }
            }

            else
            {
              printf("Directory doesn't exist\n");
              printf("Press enter...");
              fgets(delaying_input, 2, stdin);
            }
          }

          /** Moving up one level option */
          else if(strcmp(option_input, up_dir) == 0)
          {
            char directory[40];


            /** Storing input */
            printf("Move to directory: ");
            fgets(directory, MAX_CHAR, stdin);
            strtok(directory, "\n");

            /** Checking if directory exists */
            if (access(directory, F_OK) == 0)
            {
              chdir(directory);
            }

            else
            {
              printf("Directory doesn't exist\n");
              printf("Press enter...");
              fgets(delaying_input, 2, stdin);
            }
          }

          /** Moving down one level option */
          else if(strcmp(option_input, down_dir) == 0)
          {
            chdir("..");
          }


          else if (strcmp(option_input, "output_svg") == 0)
          {
            char graph_dir[50];
            char directory[100];
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
            char input[40];
            char cut_result[50];
            char cut_trimmed_result[40];
            char cut_str[40];
            int count = 0;
            int num;
            int i = 0;
            int j = 0;


            /** Storing input */
            printf("Directory to graph: ");
            fgets(graph_dir, MAX_CHAR, stdin);
            strtok(graph_dir, "\n");

            /** Checking if directory exists */
            if (access(graph_dir, F_OK) == 0)
            {

              /** Installing plantuml */
              system("sudo apt install -y graphviz default-jre; mkdir -p .opt/plantuml; cd .opt/plantuml; sudo curl -JLO http://sourceforge.net/projects/plantuml/files/plantuml.jar/download;");

              FILE *fp = fopen("tree_diagram.txt", "w"); /** Creating a file */

              snprintf(directory, sizeof(directory), "./%s", graph_dir); /** String construction */

              int length = strlen(directory); /** Length of the string */

              /** Copying string */
              for(int i = 0; i < length; i++)
              {
                input[i] = directory[i];
              }

              /** Copying string */
              memset(cut_str, '\0', sizeof(cut_str));
              strncpy(cut_str, input, length);


              /** Iterating through the current directory's directories */
              for (int i=0; i<list_file_type(1, result_list, 0, "."); i++)
              {
                snprintf(results, sizeof(results), "%s", result_list[i]); /** Constructing the path */

                /** Copying string with limited length */
                for(int i = 0; i < length; i++)
                {
                  cut_result[i] = results[i];
                }

                /** Copying string */
                memset(cut_trimmed_result, '\0', sizeof(cut_trimmed_result));
                strncpy(cut_trimmed_result, cut_result, length);


                /** if input path contains part of input execute code */
                if (strcmp(cut_trimmed_result, cut_str) == 0)
                {

                  count = 0;
                  j = 0;
                  num_of_slashes = 0;
                  strcpy(variable2, "");

                  /** Iterating through the characters */
                  while (results[count] != '\0')
                  {
                    /** If symbol / found */
                    if (results[count] == '/')
                    {
                      num_of_slashes++; /** level of directory depth */
                    }
                    count++;
                  }

                  strcpy(new_str, results);

                  char *bin;
                  char *docs;
                  char *lib;
                  char *src;
                  char *tests;
                  bin = strstr(new_str, "bin");
                  docs = strstr(new_str, "docs");
                  lib = strstr(new_str, "lib");
                  src = strstr(new_str, "src");
                  tests = strstr(new_str, "tests");


                  /** Excluding bin, docs, lib, src and tests folders */
                  if (bin || docs || lib || src || tests)
                  {
                  }

                  else
                  {
                    token[j] = strtok(new_str, "/"); /** Dividing a string by symbol "/" into tokens */

                    /** Iterating through the string */
                    while (token[j] != NULL)
                    {
                      token[++j] = strtok(NULL, "/");
                    }
                    snprintf(variable, sizeof(variable), "%s", token[j-1]); /** Constructing the string */

                    /** Constructing number of levels in the directory */
                    for (num = 0; num < num_of_slashes; num++)
                    {
                      strcat(variable2, "*");
                    }

                    snprintf(message, sizeof(message)+50, "%s %s\n", variable2, variable);
                    strcat(constructed_str, message);
                  }
                }
              }

              char mess[500];
              char start_temp[20] = "@startwbs";
              char end_temp[20] = "@endwbs";
              int k = 0;

              /** Shifting 1 file's position */
              while (constructed_str[k] != '*')
              {
                memmove(constructed_str, constructed_str+1, strlen(constructed_str));
              }

              /** Constructing a final string in plantuml format */
              snprintf(mess, 500, "%s\n%s%s\n", start_temp, constructed_str, end_temp);

              fputs(mess, fp); /** Writing string to file */
              fclose(fp); /** Colsing the file */

              /** Processing txt file through plantuml */
              system("java -jar .opt/plantuml/plantuml.jar tree_diagram.txt");

              int flag = 0;

              if (access("tree_diagram.png", F_OK) != 0)
              {
                flag = 1;
                printf("\n\nSomething went wrong\n");
                printf("Press enter...");
                fgets(delaying_input, 2, stdin);
              }

              if (flag == 0)
              {
                printf("\n\nWBS Tree diagram output successful!\n");
                printf("Press enter...");
                fgets(delaying_input, 2, stdin);
              }
            }

            else
            {
              printf("Directory doesn't exist\n");
              printf("Press enter...");
              fgets(delaying_input, 2, stdin);
            }
          }
        }
      }

      else if (strcmp(user_input, create_proj) == 0)
      {
        creating_directory(".");
      }

      else if (strcmp(user_input, create_git) == 0) /** Checking for command match */
      {
        system("git init; git remote add origin https://csgitlab.ac.uk/nn020334/pc20_pm_cw2.git"); /** Initializing git */
      }

      system("clear");
    }
  }

  /** Features 1,2,3 | If command "create_project" - creating new project */
  else if (strcmp(argv[1], "create_project") == 0)
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


  /** Features 1,2,3 | If command "add_feature" - creating new feature */
  else if (strcmp(argv[1], "add_feature") == 0)
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


  /** Feature 4 | If command "add_tag" - adding a new tag */
  else if (strcmp(argv[1], "add_tag") == 0)
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



  /** Feature 4 | If command "find_tag" - finding a tag */
  else if (strcmp(argv[1], "find_tag") == 0)
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

      /** Checking for existing folders or files */
      if (access(".pm_tag", F_OK) == 0)
      {
        FILE *fp;
        char tag[20];

        fp = fopen(".pm_tag", "r"); /** Opening a file */
        fseek(fp, 0, SEEK_END); /** Setting file's position to the end of the stream */
        long int len = ftell(fp); /** Storing current file's position in stream */

        /** If file's position more than 0 i.e. there is content */
        if(len > 0)
        {
          rewind(fp); /** Rewinding file's position in stream */
          fgets(tag, sizeof(tag), fp); /** storing contents in a variable */
          strtok(tag, "\n");
        }

        /** If tag equals to input tag */
        if (strcmp(argv[2], tag) == 0)
        {
          local_tag++; /** marking tag's detection */
          printf("%s\n", "./");
        }
        fclose(fp); /** Closing a file */
      }

      /** If tag was not detected i.e. less than 1 */
      if (local_tag < 1)
      {
        /** iterating through the path's of directories inside current directory */
        for (int i=0; i<list_file_type(1, result_list, 0, "."); i++)
        {
          snprintf(path, sizeof(path), "%s/.pm_tag", result_list[i]); /** Constructing a string */

          /** searching for a file */
          if (access(path, F_OK) == 0)
          {
            FILE *fp;
            char tag[20];

            fp = fopen(path, "r"); /** opening a file */
            fseek(fp, 0, SEEK_END); /** Setting file's position to the end of the stream */
            long int len = ftell(fp); /** Storing current file's position in a stream */

            /** If file's position more than 0 i.e. there is content */
            if(len > 0)
            {
              rewind(fp); /** Rewinding file's position */
              fgets(tag, 20, fp); /** Storing contents in a variable */
              strtok(tag, "\n");
            }

            /** If tag equals to input tag */
            if (strcmp(argv[2], tag) == 0)
            {
              found = 1; /** Marking tag's detection */
              printf("%s\n", result_list[i]);
            }
            fclose(fp); /** Closing a file */
          }
        }

        /** if tag not found */
        if (found == 0)
        {
          printf("Tag not found\n");
        }
      }
    }
  }

  /** Feature 5 | If command "rename_directory" - renaming existing directory */
  else if (strcmp(argv[1], "rename_directory") == 0)
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
      rename(argv[2], argv[3]); /** Renaming directory */
      printf("Directory successfully renamed!\n");
    }

    else /** If directory not found - Poping an error message */
    {
      printf("Couldn't find directory: %s\n", argv[2]);
    }

  }

  /** Feature 6 | If command "move_by_tag" - creating new project */
  else if (strcmp(argv[1], "move_by_tag") == 0)
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


      /** Checking for existing file */
      if (access(".pm_tag", F_OK) == 0)
      {
        FILE *fp;
        char tag[20];

        fp = fopen(".pm_tag", "r"); /** Opening a file */
        fseek(fp, 0, SEEK_END); /** Setting file's position to the end of file */
        long int len = ftell(fp); /** Storing file's position */

        /** If file's position more than zero */
        if(len > 0)
        {
          rewind(fp); /** Rewinding file's position */
          fgets(tag, 20, fp); /** Storing file's contents */
          strtok(tag, "\n");
        }

        /** If tag found in third argument */
        if (strcmp(argv[2], tag) == 0)
        {
          cur_dir = 1;
          found += 2;
          printf("Cannot move current directory elsewhere\n");
        }

        /** If tag found in fourth argument */
        else if (strcmp(argv[3], tag) == 0)
        {
          found++;
          snprintf(arg3_tag, sizeof(arg3_tag), "./");
        }
        fclose(fp); /** Closing file */
      }

      /** If tag was not found in current directory */
      if (cur_dir == 0)
      {
        /** iterating through the path's of directories inside current directory */
        for (int i=0; i<list_file_type(1, result_list, 0, "."); i++)
        {
          snprintf(path, sizeof(path), "%s/.pm_tag", result_list[i]);

          /** Checking for tag's file */
          if (access(path, F_OK) == 0)
          {
            FILE *fp;
            char tag[20];

            /** Opening and storing file's position */
            fp = fopen(path, "r");
            fseek(fp, 0, SEEK_END);
            long int len = ftell(fp);

            /** Checking if file has contents */
            if(len > 0)
            {
              rewind(fp);
              fgets(tag, 20, fp);
              strtok(tag, "\n");
            }

            /** Checking for tag in third argument */
            if (strcmp(argv[2], tag) == 0)
            {
              found += 2;
              snprintf(arg2_tag, sizeof(arg2_tag), "%s", result_list[i]); /** Constructing a string */
            }

            /** Checking for tag in fourth argument */
            else if (strcmp(argv[3], tag) == 0)
            {
              found++;
              snprintf(arg3_tag, sizeof(arg3_tag), "%s", result_list[i]); /** Constructing a string */
            }
            fclose(fp); /** Closing a file */
          }
        }

        /** if 0 tags found */
        if (found == 0)
        {
          printf("Tags not found\n");
        }

        /** if only second tag found */
        if (found == 1)
        {
          printf("First tag not found\n");
        }

        /** if only first tag found */
        if (found == 2)
        {
          printf("Second tag not found\n");
        }

        /** if both tags found */
        if (found == 3)
        {
          char *directory_str[30];
          char dir_path[120];
          char arg2[50];
          strcpy(arg2, arg2_tag);
          int i = 0;


          directory_str[i] = strtok(arg2_tag, "/"); /** dividing string by / into tokens */

          /** iterating through the tokens */
          while (directory_str[i] != NULL)
          {
            directory_str[++i] = strtok(NULL, "/");
          }

          snprintf(dir_path, sizeof(dir_path), "%s/%s", arg3_tag, directory_str[i-1]); /** Constructing the string */

          rename(arg2, dir_path); /** Transfering the directory */
          printf("Directory successfully moved!\n");
        }
      }
    }
  }


  /** Feature 7 | If command "output_svg" - creating WBS tree diagram */
  else if (strcmp(argv[1], "output_svg") == 0)
  {
    if (argc > 3 || argc < 3 || strcmp(argv[2], "") == 0) /** Checking for unallowed number of arguments */
    {
      printf("Wrong number of parameters, should be pm command directory\n");
    }

    else if (strchr(argv[2], ' ') != 0) /** Checking for unallowed spaces in the 3rd argument */
    {
      printf("Bad characters in folder name\n");
    }

    else if (access(argv[2], F_OK) == 0) /** Checking for existing file */
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
      char input[40];
      char cut_result[50];
      char cut_trimmed_result[40];
      char cut_str[40];
      int count = 0;
      int num;
      int i = 0;
      int j = 0;

      /** Installing plantuml */
      system("sudo apt install -y graphviz default-jre; mkdir -p .opt/plantuml; cd .opt/plantuml; sudo curl -JLO http://sourceforge.net/projects/plantuml/files/plantuml.jar/download;");

      FILE *fp = fopen("tree_diagram.txt", "w"); /** Creating a file */

      snprintf(directory, sizeof(directory), "./%s", argv[2]); /** String construction */

      int length = strlen(directory); /** Length of the string */

      /** Copying string */
      for(int i = 0; i < length; i++)
      {
        input[i] = directory[i];
      }

      /** Copying string */
      memset(cut_str, '\0', sizeof(cut_str));
      strncpy(cut_str, input, length);


      /** Iterating through the current directory's directories */
      for (int i=0; i<list_file_type(1, result_list, 0, "."); i++)
      {
        snprintf(results, sizeof(results), "%s", result_list[i]); /** Constructing the path */

        /** Copying string with limited length */
        for(int i = 0; i < length; i++)
        {
          cut_result[i] = results[i];
        }

        /** Copying string */
        memset(cut_trimmed_result, '\0', sizeof(cut_trimmed_result));
        strncpy(cut_trimmed_result, cut_result, length);

        /** if input path contains part of input execute code */
        if (strcmp(cut_trimmed_result, cut_str) == 0)
        {
          count = 0;
          j = 0;
          num_of_slashes = 0;
          strcpy(variable2, "");

          /** Iterating through the characters */
          while (results[count] != '\0')
          {
            /** If symbol / found */
            if (results[count] == '/')
            {
              num_of_slashes++; /** level of directory depth */
            }
            count++;
          }

          strcpy(new_str, results);

          char *bin;
          char *docs;
          char *lib;
          char *src;
          char *tests;
          bin = strstr(new_str, "bin");
          docs = strstr(new_str, "docs");
          lib = strstr(new_str, "lib");
          src = strstr(new_str, "src");
          tests = strstr(new_str, "tests");


          /** Excluding bin, docs, lib, src and tests folders */
          if (bin || docs || lib || src || tests)
          {
          }
          else
          {
            token[j] = strtok(new_str, "/"); /** Dividing a string by symbol "/" into tokens */

            /** Iterating through the string */
            while (token[j] != NULL)
            {
              token[++j] = strtok(NULL, "/");
            }
            snprintf(variable, sizeof(variable), "%s", token[j-1]); /** Constructing the string */

            /** Constructing number of levels in the directory */
            for (num = 0; num < num_of_slashes; num++)
            {
              strcat(variable2, "*");
            }

            snprintf(message, sizeof(message)+50, "%s %s\n", variable2, variable);
            strcat(constructed_str, message);
          }
        }
      }

      char mess[500];
      char start_temp[20] = "@startwbs";
      char end_temp[20] = "@endwbs";
      int k = 0;

      /** Shifting 1 file's position */
      while (constructed_str[k] != '*')
      {
        memmove(constructed_str, constructed_str+1, strlen(constructed_str));
      }

      /** Constructing a final string in plantuml format */
      snprintf(mess, 500, "%s\n%s%s\n", start_temp, constructed_str, end_temp);

      fputs(mess, fp); /** Writing string to file */
      fclose(fp); /** Colsing the file */

      /** Processing txt file through plantuml */
      system("java -jar .opt/plantuml/plantuml.jar tree_diagram.txt");
      printf("WBS Tree diagram output successful!\n");
    }

    /** If specified directory not found */
    else
    {
      printf("Couldn't find directory: %s\n", argv[2]);
    }
  }


  else if (strcmp(argv[1], "calculate_workload") == 0)
  {
    printf("Feature\n");
  }

  else
  {
    printf("Wrong command or number of parameters, should be pm command arg\n");
  }

  return 0;

}
