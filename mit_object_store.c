#include <dirent.h>
#include <string.h>

#include "sha1.h"
#include "mit_object_store.h"

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
  unsigned char        c;                  /* Character read from file      */
  unsigned int num_chars_to_read = 1;

  SHA1Reset(&sha);

  c = fgetc(data);
  while(!feof(data)) {
    SHA1Input(&sha, &c, num_chars_to_read);
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
  int chars_to_read;

  if (!(file = fopen("./.mit/index","r"))) {
    fprintf(stderr, "Unable to read the index file\n");
  } else {
    while(fgets(buffer, buffer_size, file)) {
      line_length = strlen(buffer);
      chars_to_read = line_length - prefix_length - 1 - 40 - 1;
      strncpy(index_entry_filename, &buffer[9], chars_to_read);
      index_entry_filename[chars_to_read] = '\0';
      if(strcmp(index_entry_filename, filename) == 0) {
        strncpy(object_hash, &buffer[line_length - 41], 40);
        hash_found = true;
      }
    }

    fclose(file);
    file = NULL;
  }

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
  int chars_to_read;


  char branch_name[1024];
  
  get_current_branch(branch_name);
  
  char branch_head_filename[1024];
  sprintf(branch_head_filename, ".mit/%s", branch_name);
  
  if (!(file = fopen(branch_head_filename,"r"))) {
    
  } else {
    while(fgets(buffer, buffer_size, file)) {
      line_length = strlen(buffer);
      chars_to_read = line_length - prefix_length - 1 - 40 - 1;
      strncpy(index_entry_filename, &buffer[9], chars_to_read);
      index_entry_filename[chars_to_read] = '\0';
      if(strcmp(index_entry_filename, filename) == 0) {
        strncpy(object_hash, &buffer[line_length - 41], 40);
        hash_found = true;
      }
    }

    fclose(file);
    file = NULL;
  }

  return hash_found;
}


void print_index_contents() {
  FILE* file;
  int buffer_size = 1024;
  int prefix_length = 9;
  char buffer[buffer_size];
  char index_entry_filename[1024];
  int line_length;
  int chars_to_read;
  char current_index_content[41];
  char current_head_hash[41];
  bool hash_found_in_head = false;
  bool print_header = true;

  if (!(file = fopen(".mit/index","r"))) {
  } else {
    while(fgets(buffer, buffer_size, file)) {
      line_length = strlen(buffer);
      chars_to_read = line_length - prefix_length - 1 - 40 - 1;
      strncpy(index_entry_filename, &buffer[9], chars_to_read);
      index_entry_filename[chars_to_read] = '\0';
      strncpy(current_index_content, &buffer[line_length - 41], 40);
      hash_found_in_head = get_hash_from_head(current_head_hash, index_entry_filename);
      if(hash_found_in_head) {
        if(strcmp(current_index_content, current_head_hash) == 0) {
          
        } else {
          if(print_header) {
            printf("# Changes to be committed:\n#\n");
            print_header = false;
          }
          printf("#\tModified file:\t%s\n", index_entry_filename);
        }
      } else {
        if(print_header) {
          printf("# Changes to be committed:\n#\n");
          print_header = false;
        }
        printf("#\tNew file:\t%s\n", index_entry_filename);
      }
    }
    printf("#\n");
    fclose(file);
    file = NULL;
  }

  if(print_header) {
    printf("Nothing to commit\n");
  }
}
