#include "utils.h"
#include "kv_table.h"
#include "libs.h"

#ifndef CLI_TABLE_H
# define CLI_TABLE_H

#include "cli_colors.h"

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

extern t_kv_table *command_table;

int     init_cli(void);
void	run_cli(t_kv_store *table);
void	exec_cmd(t_kv_store *table, int argc, char **argv);
void	clear_console(void);
void	pprompt(const char *table_name);

int				init_command_table(void);
t_status_code	register_command(t_command *cmd);
t_command		*find_command(const char *name);
void			cleanup_command_sys(void);

void    display_table(t_data_table table);
void    display_header(t_data_table table);

void    init_data_table(t_data_table *data_table, const char *title, const char *column_titles[]);
int     set_title(t_data_table *table, const char *title);
void    add_column(t_data_table *table, const char *title);
void	cleanup_data_table(char ***data, int row);

void	handle_entries(t_kv_store *store, int argc, char **argv);
void    handle_list(t_kv_store *store, int argc, char **argv);
void    handle_switch(t_kv_store *store, int argc, char **argv);

int		kv_table_commands(void);
int		kv_store_commands(void);
int		utility_commands(void);

#endif