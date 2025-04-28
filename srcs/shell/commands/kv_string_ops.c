#include "kv_store.h"
#include "kv_table.h"
#include "shell.h"

void    handle_incr(t_kv_store *store, int argc, char **argv)
{
    t_status_code   status;

    if (argc != 2)
    {
        logger(2, "Usage: incr <key>");
        return ;
    }
    status = kv_incr(store->table, argv[1]);
    log_message(1, status);
}

void    handle_decr(t_kv_store *store, int argc, char **argv)
{
    t_status_code   status;

    if (argc != 2)
    {
        logger(2, "Usage: decr <key>");
        return ;
    }
    status = kv_decr(store->table, argv[1]);
    log_message(1, status);
}

void    handle_incr_by(t_kv_store *store, int argc, char **argv)
{
    t_status_code	status;
	int				number;
    char			*endptr;

    if (argc != 3)
    {
        logger(2, "Usage: incr <key> <increment>");
        return ;
    }
    number = strtol(argv[2], &endptr, 10);
	if (endptr == argv[2] || *endptr != '\0')
	{
		logger(1, "Error: Invalid number provided for increment value");
		return ;
	}
    status = kv_incr_by(store->table, argv[1], number);
    log_message(1, status);
}

void    handle_decr_by(t_kv_store *store, int argc, char **argv)
{
    t_status_code	status;
	int				number;
    char			*endptr;

    if (argc != 3)
    {
        logger(2, "Usage: decr <key> <increment>");
        return ;
    }
    number = strtol(argv[2], &endptr, 10);
	if (endptr == argv[2] || *endptr != '\0')
	{
		logger(1, "Error: Invalid number provided for increment value");
		return ;
	}
    status = kv_decr_by(store->table, argv[1], number);
    log_message(1, status);
}

int kv_string_ops_commands(void)
{
    size_t      i;
    t_status_code   status;
    static t_command    commands[] = {
        {
            .name = "incr",
            .usage = "incr <key>",
            .description = "",
            .handler = handle_incr,
            .flags = TABLE_REQUIRED
        },
        {
            .name = "decr",
            .usage = "decr <key>",
            .description = "",
            .handler = handle_decr,
            .flags = TABLE_REQUIRED
        },
        {
            .name = "incrby",
            .usage = "incrby <key> <increment>",
            .description = "",
            .handler = handle_incr_by,
            .flags = TABLE_REQUIRED
        },
        {
            .name = "decrby",
            .usage = "decrby <key> <increment>",
            .description = "",
            .handler = handle_decr_by,
            .flags = TABLE_REQUIRED
        },
    };

    i = 0;
    while (i < sizeof(commands) / sizeof(commands[0]))
    {
        status = register_command(&commands[i]);
        if (status != SUCCESS_CODE)
            return (1);
        i++;
    }
    return (0);
}