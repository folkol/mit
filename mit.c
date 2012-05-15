#include <stdio.h>

int main(int num_args, char** args) {

  if (num_args < 2) {
    printf("Usage: mit <command>\n");
  } else {
    printf("Output of: %s\n", args[1]);
  }

  return 0;
}
