#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h>

#include "mit_commands.h"
#include "mit_object_store.h"


void command_init() {
  printf("Initializing mit repository!\n");
  mkdir("./.mit", 0777);
  mkdir("./.mit/objects", 0777);
  mkdir("./.mit/refs", 0777);
  mkdir("./.mit/refs/heads", 0777);

  FILE* file;
  if (!(file = fopen("./.mit/HEAD","w"))) {
    fprintf(stderr, "Unable to create the HEAD file");
  }
  fprintf(file, "refs/heads/master");
  fclose(file);
  file = NULL;
}


void command_status() {
  char current_branch[255];
  get_current_branch(current_branch);
  printf("# On branch %s\n", current_branch);
  print_index_contents();
}


void command_add(int num_opts, char** opts) {
  int c = 0;
  char object_hash[41];
  for(c = 0; c < num_opts; c++) {
    command_add_single_entry(object_hash, opts[c]);
  }
}

void command_add_single_entry(char* object_hash, char* filename) {
    store_blob(object_hash, filename);
}


void command_restore(int num_args, char** args) {
  int c = 0;
  if(num_args < 2) {
    printf("Too few arguments!\n");
    printf("usage: mit restore <object_hash> <filename>\n");
    exit(-1);
  }
  for(c = 0; c < num_args; c++) {
    command_restore_single_entry(args[0], args[1]);
  }
}


void command_restore_single_entry(char* object_hash, char* filename) {
  char object_filename[13+41];

  sprintf(object_filename, ".mit/objects/%s", object_hash);
  FILE* blob_file = fopen(object_filename, "r");
  if(!blob_file) {
    printf("The file is not currently under version control!\n");
    return;
  }

  FILE* target_file = fopen(filename, "w");
  char        c;                  /* Character read from file      */

  c = fgetc(blob_file);
  while(!feof(blob_file)) {
    fputc(c, target_file);
    c = fgetc(blob_file);
  }

  fclose(target_file);
  fclose(blob_file);
}


void command_checkout(int num_args, char** args) {
  int c = 0;
  if(num_args == 0) {
    printf("Too few arguments!\n");
    printf("usage: mit checkout <filename>\n");
    exit(-1);
  }

  for(c = 0; c < num_args; c++) {
    command_checkout_single_entry(args[c]);
  }
}


void command_checkout_single_entry(char* filename) {
  char object_filename[255];
  char object_hash[41];
  bool hash_found = false;
  hash_found = get_hash_from_index(object_hash, filename);

  if(!hash_found) {
    hash_found = get_hash_from_head(object_hash, filename);
  }

  if(!hash_found) {
    printf("The file is not currently under version control!\n");
    return;
  }

  sprintf(object_filename, ".mit/objects/%s", object_hash);
  FILE* blob_file = fopen(object_filename, "r");
  if(!blob_file) {
    printf("A object hash was found, but we could not open the file. Object store might be corrupted!\n");
    return;
  }

  FILE* target_file = fopen(filename, "w");
  char        c;                  /* Character read from file      */

  c = fgetc(blob_file);
  while(!feof(blob_file)) {
    fputc(c, target_file);
    c = fgetc(blob_file);
  }

  fclose(target_file);
  fclose(blob_file);
}

void command_commit() {
  char branch_name[1024];
  char branch_head_filename[1024];

  get_current_branch(branch_name);

  sprintf(branch_head_filename, ".mit/%s", branch_name);

  FILE* index_file = fopen(".mit/index", "r");
  if(!index_file) {
    printf("No indexfile found\n");
    return;
  }

  FILE* head_file = fopen(branch_head_filename, "w");
  if(!head_file) {
    printf("Could not open headfile\n");
    return;
  }
  char        c;                  /* Character read from file      */

  c = fgetc(index_file);
  while(!feof(index_file)) {
    fputc(c, head_file);
    c = fgetc(index_file);
  }

  fclose(index_file);
  fclose(head_file);
  unlink(".mit/index");
}
