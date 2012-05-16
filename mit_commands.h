#ifndef MIT_COMMANDS_H
#define MIT_COMMANDS_H

void command_init();
void command_status();
void command_add(int num_args, char** args);
void command_add_single_entry(char* object_hash, char* filename);
void command_restore(int num_args, char** args);
void command_restore_single_entry(char* object_hash, char* args);
void command_checkout(int num_args, char** args);
void command_checkout_single_entry(char* filename);
void command_commit();

#endif
