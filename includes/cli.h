#include "stdlib.h"
#include "kv_store.h"
#include "libs.h"

#ifndef CLI_TABLE_H
# define CLI_TABLE_H

#define DEF_COL_WIDTH 10
#define MAX_COLS 15

typedef struct s_table_col {
    char    title[64];
    int     width;
} t_table_col;

typedef struct s_table_header {
    t_table_col columns[10];
    int         column_count;
} t_table_header;

typedef struct s_table_header_item {
    char    name[50];
} t_table_header_item;

typedef struct s_data_table {
	char				*title;
    char				***data;
    int					column_count;
    t_table_col			columns[MAX_COLS];
} t_data_table;

void    display_table(t_data_table table);
void    display_header(t_data_table table);

void    init_data_table(t_data_table *data_table, const char *title, const char *column_titles[]);
int     set_title(t_data_table *table, const char *title);
void    add_column(t_data_table *table, const char *title);
void	cleanup_data_table(char ***data, int row); 

void	cmd_entries(t_kv_store *store, int argc, char **argv);
void    cmd_list(t_kv_store *store, int argc, char **argv);
void    cmd_exit(t_kv_store *store, int argc, char **argv);
void    cmd_switch(t_kv_store *store, int argc, char **argv);

#endif