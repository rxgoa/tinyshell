#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

double BytesToMegaBytes(unsigned long bytes);

// return int code after calling main()
int main(int argc, char *argv[]){
  if(argc == 1)
    printf("\nNo extra argument passed for the program.\n");

  // we start looping excluding the first index (0) since it will always be the file itself "./tinyshell"
  for (int i = 1; i < argc; i++) {
    // check if help command was typed
    if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
      // check if there was more than 1 argument passed, if so, we show a warning
      if(argc - 1 > 1)
        printf("%s\n", "Command help don't accept arguments.");
      else {      
        printf("%s\n", "Commands available:");
        // FIXME: we should call our commands available function so we don't need to hard code this.
        printf("%s\n", "ls --> It is designed to list the names and features of files and directories.");
        printf("%s\n", "cd --> Allows you to move between directories.");
        printf("%s\n", "cat --> Show contents for a specific file.");
      }

    if(strcmp(argv[i], "ls") == 0){
      struct dirent **namelist;
      int n; // this variable will hold the number of items inside my dir 

      // scandir(PATH_TO_SCAN, POINTER_TO_HOLD_VALUES_OF_OUTPUT, FILTERING_RULES, ALPHA_SORTING)
      n = scandir(".", &namelist, NULL, alphasort);
      //alphasort is a comparison function used with scandir to sort directory entries alphabetically by name. 
      // It's one of the functions provided by the C standard library, specifically intended for use as a sorting function in scandir.
      
      if(n < 0){
        perror("scandir");
        return EXIT_FAILURE;
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

    if(strcmp(argv[i], "cd") == 0)
      printf("%s\n", "cd command requested");
    if(strcmp(argv[i], "cat") == 0)
      printf("%s\n", "cat command requested");
  }
  return 0;
}

double BytesToMegaBytes(unsigned long bytes){
  return bytes / (1024.0 * 1024.0);
}
