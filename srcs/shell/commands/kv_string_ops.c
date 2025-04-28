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
        }
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