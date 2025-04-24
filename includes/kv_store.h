#ifndef KV_STORE_H
# define KV_STORE_H

#include "status_codes.h"

#define DIR_DATA "./data/"

typedef struct s_kv_table t_kv_table;

typedef struct s_kv_store
{
	t_kv_table	*table;
	char		name[50];
} t_kv_store;

t_status_code	tb_create(char *filename);
t_status_code   tb_drop(char *filename);
t_status_code   tb_rename(char *old, char *new);

#endif