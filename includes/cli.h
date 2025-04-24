#include "utils.h"
#include "kv_table.h"
#include "libs.h"

#ifndef CLI_TABLE_H
# define CLI_TABLE_H

#include "cli_colors.h"

#define DEF_COL_WIDTH 10
#define MAX_COLS 15
// #define TABLE_REQUIRED 0x1
// #define NO_TABLE_ALLOWED 0x2

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

typedef void    (*cmd_handler)(t_kv_store *, int, char **);

typedef enum {
    NO_FLAG = 0,
    TABLE_REQUIRED = 1,
    NO_TABLE_ALLOWED = 2
} t_cmd_flags;

typedef struct s_command
{
	const char  *name;
    const char  *usage;
    const char  *description;
    cmd_handler handler;
    t_cmd_flags flags;
} t_command;

void	run_cli(t_kv_store *table);
void	exec_cmd(t_kv_store *table, int argc, char **argv);
void	clear_console(void);
void	pprompt(const char *table_name);

void        register_command(t_command *cmd);
t_command	*find_command(const char *name);
void        init_command_sys(void);
void        cleanup_command_sys(void);

void    display_table(t_data_table table);
void    display_header(t_data_table table);

void    init_data_table(t_data_table *data_table, const char *title, const char *column_titles[]);
int     set_title(t_data_table *table, const char *title);
void    add_column(t_data_table *table, const char *title);
void	cleanup_data_table(char ***data, int row);

void	handle_entries(t_kv_store *store, int argc, char **argv);
void    handle_list(t_kv_store *store, int argc, char **argv);
void    handle_switch(t_kv_store *store, int argc, char **argv);

void	register_kv_table_cmds(void);
void 	register_kv_store_cmds(void);
void	register_all_commads(void);

#endif