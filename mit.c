#include <stdio.h>

#include "mit_opts.h"

mit_opts* example_opts = NULL;

/*
 * Clean resources
 */
void cleanup() {
  free_opts();
}

int main(int num_args, char** args) {
  example_opts = parse_opts(num_args, args);

  if (num_args < 2) {
    printf("Usage: mit <command>\n");
  } else {
    printf("Output of: %s\n", args[1]);
  }

  cleanup();
  return 0;
}
