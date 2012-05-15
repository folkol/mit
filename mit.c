#include <stdio.h>
#include <stdlib.h>

// Print usage and exit
void usage();

int main(int num_args, char** args) {

  if (num_args < 2) {
    usage();
  }

  return 0;
}

void usage() {
  printf("Usage: mit <command>\n");
  exit(0);
}
