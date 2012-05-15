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
char* get_current_branch();
void command_init();
void command_status();
void command_add(int num_args, char** args);
char* store_blob(const char* filename);

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
  char* current_branch = get_current_branch();
  printf("# On branch %s\n", current_branch);
  printf("# Changes to be committed:\n");
  printf("#\n");
  printf("# Untracked files:\n");
  printf("#\n");
}


void command_add(int num_opts, char** opts) {
  printf("Updating index...\n");
  int c = 0;
  for(c = 0; c < num_opts; c++) {
    printf("Updating index for %s\n", opts[c]);
    store_blob(opts[c]);
  }
}


char* store_blob(const char* filename) {
  SHA1Context sha;                /* SHA-1 context                 */
  FILE        *fp;                /* File pointer for reading files*/
  char        c;                  /* Character read from file      */
  int         i;                  /* Counter                       */
  int         reading_stdin;      /* Are we reading standard in?   */
  int         read_stdin = 0;     /* Have we read stdin?           */

  char sha_part_one[9];
  char sha_part_two[9];
  char sha_part_three[9];
  char sha_part_four[9];
  char sha_part_five[9];

  char object_hash[41];

  if (!(fp = fopen(filename,"rb")))
    {
      fprintf(stderr,
              "sha: unable to open file %s\n",
              filename);
    }

  /*
   *  Reset the SHA-1 context and process input
   */
  SHA1Reset(&sha);

  c = fgetc(fp);
  while(!feof(fp))
    {
      SHA1Input(&sha, &c, 1);
      c = fgetc(fp);
    }

  if (!reading_stdin)
    {
      fclose(fp);
    }

  if (!SHA1Result(&sha))
    {
      fprintf(stderr,
              "sha: could not compute message digest for %s\n",
              filename);
    }
  else
    {
      sprintf(sha_part_one, "%08X", sha.Message_Digest[0]);
      sprintf(sha_part_two, "%08X", sha.Message_Digest[1]);
      sprintf(sha_part_three, "%08X", sha.Message_Digest[2]);
      sprintf(sha_part_four, "%08X", sha.Message_Digest[3]);
      sprintf(sha_part_five, "%08X", sha.Message_Digest[4]);
    }

  fclose(fp);


  sprintf(object_hash, "%s%s%s%s%s", sha_part_one, sha_part_two, sha_part_three, sha_part_four, sha_part_five);

  printf("The calculated object hash is: %s\n", object_hash);
}
