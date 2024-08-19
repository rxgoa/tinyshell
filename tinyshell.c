#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

typedef struct {
  char *command;
} Command;

double BytesToMegaBytes(unsigned long bytes);
void ProcessCommand(char *input, char *tokens[]);
bool CheckCommand(char *tokens[]);
char* GetPath(char *tokens[]);
char* GetCommand(char *tokens[]);

// return int code after calling main()
int main(){
  char input[256]; // user input
  Command commandList;

  while(1) {
    printf("tinyshell>");
    fflush(stdout);

    if(fgets(input, sizeof(input), stdin) != NULL) {
      // Remove newline character from input if present
      size_t len = strlen(input);
      if (len > 0 && input[len - 1] == '\n') {
          input[len - 1] = '\0';
      }

      if(strcmp(input, "exit") == 0){
         // exit application
         break;
      }

      char inputcp[256];
      strcpy(inputcp, input);

      char *tokens[256];
      int a = 0;

      char *token = strtok(inputcp, " ");
      strtok(input, " ");

      while(token != NULL){
          printf("%s\n", token);
          tokens[a] = token;
          a++;
          token = strtok(NULL, " ");
      }

      tokens[a] = NULL;

      bool isValid = CheckCommand(tokens);

      if(isValid){
        ProcessCommand(input, tokens); // tokens[0] always will be the first argument
      } else {
        printf("Command invalid. Please try again or type --help.\n");
      }

    } else {
      perror("Error running tinyshell");
      break;
    }

  }

  printf("Exiting program!\n");
  return 0;

}

double BytesToMegaBytes(unsigned long bytes){
  return bytes / (1024.0 * 1024.0);
}

bool CheckCommand(char *tokens[]){
    int size = 0;
    while (tokens[size] != NULL) {
        size++;
    }

    return size <= 2;
}

char* GetPath(char *tokens[]){
    if (tokens[1] != NULL) {
        return tokens[1];
    } else {
        return ".";
    }
}

char* GetCommand(char *tokens[]){
  return tokens[0];
}

void ProcessCommand(char* input, char *tokens[]) {
    if(strcmp(input, "--help") == 0 || strcmp(input, "-h") == 0){
      // check if there was more than 1 argument passed, if so, we show a warning
        printf("%s\n", "Commands available:");
        // FIXME: we should call our commands available function so we don't need to hard code this.
        printf("%s\n", "ls 'ls path/' --> It is designed to list the names and features of files and directories.");
        printf("%s\n", "cd 'cd path/'--> Allows you to move between directories.");
        printf("%s\n", "cat 'cat path_to_file/file.[extension]'--> Show contents for a specific file.");
    }

    if(strcmp(input, "ls") == 0){
          struct dirent **namelist;
          char *root_path;
          char* file_path;
          int n; // this variable will hold the number of items inside my dir

          file_path = GetPath(tokens);

          // scandir(PATH_TO_SCAN, POINTER_TO_HOLD_VALUES_OF_OUTPUT, FILTERING_RULES, ALPHA_SORTING)
          n = scandir(file_path, &namelist, NULL, alphasort);
          //alphasort is a comparison function used with scandir to sort directory entries alphabetically by name.
          // It's one of the functions provided by the C standard library, specifically intended for use as a sorting function in scandir.
          if(n < 0){
            printf("Directory is empty.\n");
          } else {
              for (int i = 0;i < n;i++) {
                struct stat fileStat;
                char full_path[100];

                if(stat(file_path, &fileStat) == 0){
                    double sizeMB = BytesToMegaBytes(fileStat.st_size);
                    printf("Name: %s | Length:%ld bytes (%.4f MB)\n", namelist[i]->d_name, fileStat.st_size, sizeMB);
                } else {
                   perror("stat");
                   strerror(errno);
                }

                free(namelist[i]);

              }

              free(namelist);

            }
    }

    if(strcmp(input, "cd") == 0)
      printf("%s\n", "cd command requested");
    if(strcmp(input, "cat") == 0){
      printf("%s\n", "cat command requested");
  }
}
