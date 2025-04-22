#ifndef KV_STORE_H
# define KV_STORE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>

#define FILE_HEADER "KVDB"

#define SUCCESS "OK."
#define ERROR_KEY_EXISTS "Error: Key already exists. Value will be updated."
#define ERROR_MEMORY_ALLOCATION "Error: Failed to allocate memory for key-value pair."
#define ERROR_MEMORY_REALLOCATION "Error: Failed to REallocate memory."
#define ERROR_INVALID_INPUT "Error: Invalid key or value format."
#define ERROR_KEY_NOT_FOUND "Error: Key not found."
#define ERROR_FILE_OPEN "Error: Unable to open file for saving data."
#define ERROR_FILE_WRITE "Error: Failed to write data to file."
#define ERROR_FILE_READ "Error: Unable to open file for loading data."
#define ERROR_FILE_FORMAT "Error: Invalid file format. Unable to parse key-value pairs."
#define ERROR_FILE_CLOSE "Error: Failed to close file."
#define ERROR_INVALID_COMMAND "Error: Invalid command. Type 'help' for a list of available commands."
#define ERROR_FILE_SAVE "Error: Failed to save data to file before exit."
#define ERROR_UNEXPECTED_EXIT "Error: An unexpected error occurred during exit. Data may not have been saved."
#define ERROR_HELP_SYNTAX "Error: Incorrect usage of the command. Use 'help' to see the correct syntax."
#define ERROR_FILE_HEADER "Error: Invalid file header."

typedef struct s_kv_pair
{
	char				key[50];
	char				value[100];
	struct s_kv_pair	*next;
} t_kv_pair;

typedef struct s_kv_table
{
	t_kv_pair	**buckets;
	int			capacity;
	int			size;
} t_kv_table;

typedef struct s_kv_store
{
	t_kv_table 	*table;
	char		name[50];
} t_kv_store;

typedef struct s_command
{
	char *cmd;
	void (*handler)(t_kv_table *, int, char **);
} t_command;

unsigned int	hash(const char *key, size_t capacity);
t_kv_table		*kv_init_table(int capacity);
void			kv_resize(t_kv_table *table);
const char		*kv_get(t_kv_table *table, const char *key);
void			kv_set(t_kv_table *table, const char *key, const char *value);
void			kv_delete(t_kv_table *table, const char *key);
void			kv_save_file(t_kv_table *table, const char *filename);
void			kv_load_file(t_kv_table *table, const char *filename);
void 			kv_render_table(t_kv_table *table);
void			run_cli(t_kv_table *table);
void			exec_cmd(t_kv_table *table, int argc, char **argv);

void			table_list(void);

#endif