#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

double BytesToMegaBytes(unsigned long bytes);
void processCommand(char *command);

// return int code after calling main()
int main(){
  char input[256]; // user input
  //
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

      processCommand(input);

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

void processCommand(char* input) {
      // check if help command was typed
    if(strcmp(input, "--help") == 0 || strcmp(input, "-h") == 0){
      // check if there was more than 1 argument passed, if so, we show a warning
        printf("%s\n", "Commands available:");
        // FIXME: we should call our commands available function so we don't need to hard code this.
        printf("%s\n", "ls --> It is designed to list the names and features of files and directories.");
        printf("%s\n", "cd --> Allows you to move between directories.");
        printf("%s\n", "cat --> Show contents for a specific file.");
    }

    if(strcmp(input, "ls") == 0){
        printf("ls command...");
        struct dirent **namelist;
        int n; // this variable will hold the number of items inside my dir

        // scandir(PATH_TO_SCAN, POINTER_TO_HOLD_VALUES_OF_OUTPUT, FILTERING_RULES, ALPHA_SORTING)
        n = scandir(".", &namelist, NULL, alphasort);
        //alphasort is a comparison function used with scandir to sort directory entries alphabetically by name.
        // It's one of the functions provided by the C standard library, specifically intended for use as a sorting function in scandir.
        if(n < 0){
          printf("Directory is empty.");
        } else {
            for (int i = 0;i < n;i++) {
              struct stat fileStat;
              if(stat(namelist[i]->d_name, &fileStat) == 0){
                  double sizeMB = BytesToMegaBytes(fileStat.st_size);
                  printf("Name: %s | Length:%ld bytes (%.4f MB)\n", namelist[i]->d_name, fileStat.st_size, sizeMB);
              } else {
                perror("stat");
              }

              free(namelist[i]);

            }
            free(namelist);
        }
    }

    if(strcmp(input, "cd") == 0)
      printf("%s\n", "cd command requested");
    if(strcmp(input, "cat") == 0)
      printf("%s\n", "cat command requested");
}
