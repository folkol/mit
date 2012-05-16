#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdbool.h>

#include "sha1.h"

void usage();
void assert_mit_repo();
void get_current_branch(char* branch_name);
void command_init();
void command_status();
void command_add(int num_args, char** args);
void command_add_single_entry(char* object_hash, char* filename);
void command_restore(int num_args, char** args);
void command_restore_single_entry(char* object_hash, char* args);
void command_checkout(int num_args, char** args);
void command_checkout_single_entry(char* filename);
void store_blob(char* object_hash, const char* filename);
void store_object(char* object_hash, FILE* file);
void get_object_hash(char* object_hash, FILE* data);
bool get_hash_from_index(char* object_hash, const char* filename);
bool get_hash_from_head(char* object_hash, const char* filename);

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
    fprintf(stderr, "This is not a .mit repository!\n");
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
  FILE* file;
  int buffer_size = 255;
  char buffer[buffer_size];
  if (!(file = fopen("./.mit/HEAD","r"))) {
    fprintf(stderr, "Unable to read the HEAD file");
  }

  fgets(buffer, buffer_size, file);

  fclose(file);
  file = NULL;
  
  sprintf(current_branch, "%s", buffer);
}


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
  fprintf(file, "refs/heads/master\n");
  fclose(file);
  file = NULL;
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
  command_restore_single_entry(args[0], args[1]);
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
  command_checkout_single_entry(args[0]);
}


void command_checkout_single_entry(char* filename) {
  char object_filename[255];
  char object_hash[41];
  bool hash_found = false;
  hash_found = get_hash_from_index(object_hash, filename);

  if(!hash_found) {
    get_hash_from_head(object_hash, filename);
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
  if (!(index_file = fopen("./.mit/index","a"))) {
    fprintf(stderr,
            "sha: unable to open the index file %s\n",
            filename);
  }
  printf("Adding %s (%s) to the index\n", filename, object_hash);
  fprintf(index_file, "Filename %s %s\n", filename, object_hash);
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


bool get_hash_from_index(char* object_hash, const char* filename) {
  FILE* file;
  int buffer_size = 1024;
  int prefix_length = 9;
  char buffer[buffer_size];
  char index_entry_filename[1024];
  int line_length;
  bool hash_found = false;

  if (!(file = fopen("./.mit/index","r"))) {
    fprintf(stderr, "Unable to read the index file");
  }

  while(fgets(buffer, buffer_size, file)) {
    line_length = strlen(buffer);
    strncpy(index_entry_filename, &buffer[9], line_length - prefix_length - 1 - 40 - 1);
    if(strcmp(index_entry_filename, filename) == 0) {
      strncpy(object_hash, &buffer[line_length - 41], 40);
      hash_found = true;
    }
  }

  object_hash = NULL;
  fclose(file);
  file = NULL;

  return hash_found;
}


bool get_hash_from_head(char* object_hash, const char* filename) {
  FILE* file;
  int buffer_size = 1024;
  int prefix_length = 9;
  char buffer[buffer_size];
  char index_entry_filename[1024];
  int line_length;
  bool hash_found = false;

  char branch_name[1024];
  
  get_current_branch(branch_name);
  printf("Attempting to retrieve object hash from branch: %s", branch_name);


  /*
  if (!(file = fopen("./.mit/index","r"))) {
    fprintf(stderr, "Unable to read the index file");
  }

  while(fgets(buffer, buffer_size, file)) {
    line_length = strlen(buffer);
    strncpy(index_entry_filename, &buffer[9], line_length - prefix_length - 1 - 40 - 1);
    if(strcmp(index_entry_filename, filename) == 0) {
      strncpy(object_hash, &buffer[line_length - 41], 40);
      hash_found = true;
    }
  }

  object_hash = NULL;
  fclose(file);
  file = NULL;

  */
  return hash_found;
}
