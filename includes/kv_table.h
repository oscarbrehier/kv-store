
#ifndef KV_TABLE_H
# define KV_TABLE_H

#include "utils.h"
#include "status_codes.h"

typedef struct s_kv_store t_kv_store;

#define FILE_HEADER "KVDB"

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

unsigned int	hash(const char *key, size_t capacity);
t_status_code	kv_init_table(t_kv_table **table, int capacity);
t_status_code	kv_get(t_kv_table *table, const char *key, const char **output);
t_status_code	kv_resize(t_kv_table *table);
t_status_code	kv_set(t_kv_table *table, const char *key, const char *value);
t_status_code	kv_delete(t_kv_table *table, const char *key);
t_status_code	kv_save_file(t_kv_table *table, const char *filename);
t_status_code	kv_load_file(t_kv_table *table, const char *filename);
void 			kv_free_table(t_kv_table *table);

#endif