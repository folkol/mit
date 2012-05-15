#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void usage();
void command_init();

int main(int num_args, char** args) {
  if (num_args < 2) {
    usage();
  }

  char* next_command;
  int c;
  for(c = 0; c < num_args; c++) {
    next_command = args[c];
    if(strcmp(next_command, "init") == 0) {
      command_init();
    }
  }

  return 0;
}


void usage() {
  printf("usage: mit <command>\n");
  exit(0);
}


void parse_directory(const char* dir) {
  DIR *dp;
  struct dirent *ep;
     
  dp = opendir (dir);
  if (dp != NULL)
    {
      while ((ep = readdir (dp)))
        puts (ep->d_name);
      (void) closedir (dp);
    }
  else
    perror ("Couldn't open the directory");
}


void command_init() {
  printf("Initializing mit repository!\n");
  mkdir("./.mit", 0777);
  mkdir("./.mit/index", 0777);
  mkdir("./.mit/objects", 0777);
}
