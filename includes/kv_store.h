#ifndef KV_STORE_H
#define KV_STORE_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>

#define TABLE_SIZE 100
#define FILE_HEADER "KVDB"

typedef struct s_kv_pair
{
	char				key[50];
	char				value[100];
	struct s_kv_pair	*next;
} t_kv_pair;

typedef struct s_command
{
	char *cmd;
	void (*handler)(t_kv_pair **, int, char **);
} t_command;

void			fn_write_error(const char *error);
int				fn_strlen(const char *str);
char			*fn_strncpy(char *dest, char *src, unsigned int n);
void			read_file_into_buffer(const char *filename, char **content);
int				is_space(char c);
int				fn_strcmp(char *s1, char *s2);
void			fn_putstr(const char *str);
int 			count_words(char *str);

unsigned int	hash(const char *key);

const char		*kv_get(t_kv_pair **table, const char *key);
void			kv_set(t_kv_pair **table, const char *key, const char *value);
void			kv_delete(t_kv_pair **table, const char *key);
void			kv_list(t_kv_pair **table);
void			kv_save_file(t_kv_pair **table, const char *filename);
void			kv_load_file(t_kv_pair **table, const char *filename);
void 			print_table(t_kv_pair **table);

void			run_cli(t_kv_pair **table);
void			exec_cmd(t_kv_pair **table, int argc, char **argv);

#endif