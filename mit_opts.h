#ifndef MIT_OPTS_H_GUARD
#define MIT_OPTS_H_GUARD

#include <stdlib.h>

/*
 * A mit_opts_node contains the name of the option
 * and it's value, if applicable
 */
typedef struct _mit_opts_option {
  char* option;
  char* value;
  struct _mit_opts_node* next;
} mit_opts_option;

/* 
 * pA mit_opts_command contains a mit target
 * and it's corresponding options
 */
typedef struct _mit_opts_command {
  char* option;
  mit_opts_option* options;
  struct _mit_opts_node* next;
} mit_opts_command;

/*
 * Contains a linked list of options passed to mit
 * And a list of commands, with corresponding options
 */
typedef struct _mit_opts {
  mit_opts_option* options;
  mit_opts_command* commands;
} mit_opts;

/*
 * parse_opts will parse the command args and create a corresponding
 */
mit_opts* parse_opts(int num_args, char** args);

/*
 * This will release all nodes in the linked lists, and NULL the lists
 */
void free_opts(mit_opts** opts);

#endif
