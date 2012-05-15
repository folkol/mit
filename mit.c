#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

void usage();
char* get_current_branch();
void command_init();
void command_status();

int main(int num_args, char** args) {
  if (num_args < 2) {
    usage();
  }

  char* next_command;
  int c;
  for(c = 1; c < num_args; c++) {
    next_command = args[c];
    if(strcmp(next_command, "init") == 0) {
      command_init();
    } else if(strcmp(next_command, "status") == 0) {
      command_status();
    } else {
      printf("%s is not a mit command!\n", next_command);
      usage();
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


char* get_current_branch() {
  return "master";
}


void command_init() {
  printf("Initializing mit repository!\n");
  mkdir("./.mit", 0777);
  mkdir("./.mit/index", 0777);
  mkdir("./.mit/objects", 0777);
  mkdir("./.mit/refs", 0777);
  mkdir("./.mit/refs/heads", 0777);
}


void command_status() {
  printf("# On branch %s\n", get_current_branch());
  printf("# Changes to be committed:\n");
  printf("#\n");
  printf("# Untracked files:\n");
  printf("#\n");
}
