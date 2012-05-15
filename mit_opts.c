#include "mit_opts.h"

/*
 * Free all nodes in the linked lists
 * and afterwards free and NULL the opts container
 */
void free_opts(mit_opts** _opts) {
  mit_opts* opts = (*_opts);
  mit_opts_node* node = NULL;
  // Free options
  while((node = opts->options)) {
    opts->options = opts->options->next;
    if(node->node) {
      free(node->node);
      node->node = NULL;
    }

    if(node->value) {
      free(node->value);
      node->value = NULL;
    }

    node = NULL;
  }

  // Free commands
  while((node = opts->options)) {
    opts->options = opts->options->next;
    if(node->node) {
      free(node->node);
      node->node = NULL;
    }

    if(node->value) {
      free(node->value);
      node->value = NULL;
    }

    node = NULL;
  }

  (*_opts) = NULL;
}

mit_opts* parse_opts(int num_args, char** args) {
  mit_opts* example_opts = (mit_opts*) malloc(sizeof(mit_opts));

  for(int i = 0; i < num_args; i++) {
    
  }

  return example_opts;
}
