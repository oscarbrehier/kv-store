
#ifndef KV_TABLE_H
# define KV_TABLE_H

#include "utils.h"
#include "status_codes.h"

typedef struct s_kv_store t_kv_store;
typedef struct s_status t_status;

#define FILE_HEADER "KVDB"
#define KEY_MIN_LEN 1
#define KEY_MAX_LEN 256
#define VAL_MIN_LEN 0
#define VAL_MAX_LEN 1024 * 1024

typedef enum s_kv_type {
	INT,
	FLOAT,
	STRING,
	BINARY,
	AUTH_DIGEST,
	COMMAND,
	UNKNOWN
} t_kv_type;

typedef struct s_kv_pair
{
	char				key[50];
	void				*value;
	size_t				value_size;
	t_kv_type			type;
	struct s_kv_pair	*next;
} t_kv_pair;

typedef struct s_kv_table
{
	t_kv_pair			**buckets;
	int					capacity;
	int					size;
	pthread_rwlock_t	rwlock;
} t_kv_table;

unsigned int	hash(const char *key, size_t capacity);
t_status		kv_init_table(t_kv_table **table, int capacity);
t_status		kv_set(t_kv_table *table, const char *key, void *value, size_t value_size, t_kv_type type);
t_status		_kv_set_internal(t_kv_table *table, const char *key, void *value, size_t value_size, t_kv_type type, int should_lock);
t_status		kv_get(t_kv_table *table, const char *key, void **output, t_kv_type type);
t_status		kv_resize(t_kv_table *table);
t_status		kv_delete(t_kv_table *table, const char *key);
t_status		kv_save_file(t_kv_table *table, const char *filename);
t_status		kv_load_file(t_kv_table *table, const char *filename);
void 			kv_free_table(t_kv_table *table);

t_status		kv_incr_by(t_kv_table *table, char *key, int increment);
t_status		kv_decr_by(t_kv_table *table, char *key, int increment);
t_status		kv_append(t_kv_table *table, char *key, char *value);
t_status		kv_strlen(t_kv_table *table, char *key, char **output);

#endif