#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mit_commands.h"
#include "mit_object_store.h"


void usage() {
  printf("usage: mit <command>\n");
  exit(0);
}


int main(int num_args, char** args) {
  if (num_args < 2) {
    usage();
  }

  char* next_command = args[1];
  if(strcmp(next_command, "init") == 0) {
    command_init();
    exit(0);
  }

  assert_mit_repo();

  if(strcmp(next_command, "status") == 0) {
    command_status();
  } else if(strcmp(next_command, "add") == 0) {
    command_add(num_args-2, &args[2]);
  } else if(strcmp(next_command, "restore") == 0) {
    command_restore(num_args-2, &args[2]);
  } else if(strcmp(next_command, "checkout") == 0) {
    command_checkout(num_args-2, &args[2]);
  } else if(strcmp(next_command, "commit") == 0) {
    command_commit();
  } else {
    printf("%s is not a mit command!\n", next_command);
    usage();
  }
  
  return 0;
}

