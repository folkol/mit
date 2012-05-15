#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "sha1.h"

void usage();
void assert_mit_repo();
void get_current_branch(char* branch_name);
void command_init();
void command_status();
void command_add(int num_args, char** args);
void store_blob(char* object_hash, const char* filename);
void store_object(char* object_hash, FILE* file);
void get_object_hash(char* object_hash, FILE* data);

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
    char** add_opts = NULL;
    if(num_args < 2) {
      add_opts = &args[2];
    }
    command_add(num_args-2, &args[2]);
  } else {
    printf("%s is not a mit command!\n", next_command);
    usage();
  }
  
  return 0;
}


void usage() {
  printf("usage: mit <command>\n");
  exit(0);
}


void assert_mit_repo() {
  DIR* mit_repo = opendir("./.mit");
  if(!mit_repo) {
    printf("This is not a .mit repository!\n");
    exit(-1);
  }
  closedir(mit_repo);
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


void get_current_branch(char* current_branch) {
  sprintf(current_branch, "master");
}


void command_init() {
  printf("Initializing mit repository!\n");
  mkdir("./.mit", 0777);
  mkdir("./.mit/objects", 0777);
  mkdir("./.mit/refs", 0777);
  mkdir("./.mit/refs/heads", 0777);
}


void command_status() {
  char current_branch[255];
  get_current_branch(current_branch);
  printf("# On branch %s\n", current_branch);
  printf("# Changes to be committed:\n");
  printf("#\n");
  printf("# Untracked files:\n");
  printf("#\n");
}


void command_add(int num_opts, char** opts) {
  int c = 0;
  char object_hash[41];
  for(c = 0; c < num_opts; c++) {
    store_blob(object_hash, opts[c]);
  }
}

void store_blob(char* object_hash, const char* filename) {
  FILE* file;
  if (!(file = fopen(filename,"rb"))) {
    fprintf(stderr,
            "sha: unable to open file %s\n",
            filename);
  }
  store_object(object_hash, file);
  fclose(file);

  FILE* index_file;
  if (!(index_file = fopen("./.mit/index","wa"))) {
    fprintf(stderr,
            "sha: unable to open the index file %s\n",
            filename);
  }
  printf("Adding %s (%s) to the index\n", filename, object_hash);
  fprintf(index_file, "Filename\t%s\t%s\n", filename, object_hash);
  fclose(index_file);
}

void store_object(char* object_hash, FILE* content) {
  get_object_hash(object_hash, content);

  char object_filename[13+41];
  sprintf(object_filename, ".mit/objects/%s", object_hash);

  FILE* object_file = fopen(object_filename, "w");
  char        c;                  /* Character read from file      */

  c = fgetc(content);
  while(!feof(content)) {
    printf("Adding char: %c", c);
    fputc(c, object_file);
    c = fgetc(content);
  }

  fclose(object_file);
}


void get_object_hash(char* object_hash, FILE* data) {
  SHA1Context sha;                /* SHA-1 context                 */
  char        c;                  /* Character read from file      */

  SHA1Reset(&sha);

  c = fgetc(data);
  while(!feof(data)) {
    SHA1Input(&sha, &c, 1);
    c = fgetc(data);
  }

  if (!SHA1Result(&sha)) {
    fprintf(stderr, "sha: could not compute message digest");
  } else {
    sprintf(object_hash, "%08x%08x%08x%08x%08x",
            sha.Message_Digest[0],
            sha.Message_Digest[1],
            sha.Message_Digest[2],
            sha.Message_Digest[3],
            sha.Message_Digest[4]);
  }

  rewind(data);
}
