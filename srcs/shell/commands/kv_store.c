#include "kv_store.h"
#include "status_codes.h"
#include "libs.h"
#include "shell.h"

void    handle_create(t_kv_store *store, int argc, char **argv)
{
    (void)store;
    t_status   status;

    if (argc != 2)
    {
        logger(2, "Usage: create <table>");
        return ;
    }
    status = tb_create(argv[1]);
    status_log(1, status.code);
}

void    handle_drop(t_kv_store *store, int argc, char **argv)
{
    (void)store;
    t_status   status;

    if (argc != 2)
    {
        logger(2, "Usage: drop <table>");
        return ;
    }
    status = tb_drop(argv[1]);
    status_log(1, status.code);
}

void    handle_rename(t_kv_store *store, int argc, char **argv)
{
    (void)store;
    t_status   status;

    if (argc != 3)
    {
        logger(2, "Usage: rename <table> <new_table_name>");
        return ;
    }
    status = tb_rename(argv[1], argv[2]);
    status_log(1, status.code);
}

void	handle_load(t_kv_store *store, int argc, char **argv)
{
	t_status	status;

	if (argc != 2)
	{
		logger(2, "Usage: save <filename>");
		return ;
	}
	status = kv_load_file(store->table, argv[1]);
	status_log(1, status.code);
}

int	kv_store_commands(void)
{
    size_t				i;
    t_status            status;
    static t_command	commands[] = {
        {
            .name = "list",
            .usage = "list",
            .description = "",
            .handler = handle_list,
            .flags = NO_TABLE_ALLOWED
        },
        {
            .name = "switch",
            .usage = "switch <table>",
            .description = "",
            .handler = handle_switch,
            .flags = NO_TABLE_ALLOWED
        },
        {
            .name = "create",
            .usage = "create <table>",
            .description = "",
            .handler = handle_create,
            .flags = NO_TABLE_ALLOWED
        },
        {
            .name = "drop",
            .usage = "drop <table>",
            .description = "",
            .handler = handle_drop,
            .flags = NO_TABLE_ALLOWED
        },
        {
            .name = "rename",
            .usage = "rename <table> <new_table_name>",
            .description = "",
            .handler = handle_rename,
            .flags = NO_TABLE_ALLOWED
        },
        {
            .name = "load",
            .usage = "load <table>",
            .description = "",
            .handler = handle_load,
            .flags = NO_TABLE_ALLOWED
        }
    };

    i = 0;
    while (i < sizeof(commands) / sizeof(commands[0]))
    {
        status = register_command(&commands[i]);
        if (status.code != SUCCESS)
		{
			return (1);
		}
        i++;
    }
	return (0);
}