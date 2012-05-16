#ifndef MIT_OBJECT_STORE_H
#define MIT_OBJECT_STORE_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void assert_mit_repo();
void get_current_branch(char* branch_name);
void store_blob(char* object_hash, const char* filename);
void store_object(char* object_hash, FILE* file);
void get_object_hash(char* object_hash, FILE* data);
bool get_hash_from_index(char* object_hash, const char* filename);
bool get_hash_from_head(char* object_hash, const char* filename);
void print_index_contents();

#endif
