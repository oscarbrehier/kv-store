#ifndef KV_STORE_H
# define KV_STORE_H

#define DIR_DATA "./data/"

typedef struct s_kv_table t_kv_table;

typedef struct s_kv_store
{
	t_kv_table 	*table;
	char		name[50];
} t_kv_store;

#endif