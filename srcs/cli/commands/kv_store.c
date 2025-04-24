#include "kv_store.h"
#include "status_codes.h"
#include "libs.h"

void    cmd_create(t_kv_store *store, int argc, char **argv)
{
    (void)store;
    t_status_code   status;

    if (argc != 2)
    {
        logger(2, "Usage: create <table>");
        return ;
    }
    status = tb_create(argv[1]);
    log_message(1, status);
}

void    cmd_drop(t_kv_store *store, int argc, char **argv)
{
    (void)store;
    t_status_code   status;

    if (argc != 2)
    {
        logger(2, "Usage: drop <table>");
        return ;
    }
    status = tb_drop(argv[1]);
    log_message(1, status);
}

void    cmd_rename(t_kv_store *store, int argc, char **argv)
{
    (void)store;
    t_status_code   status;

    if (argc != 3)
    {
        logger(2, "Usage: rename <table> <new_table_name>");
        return ;
    }
    status = tb_rename(argv[1], argv[2]);
    log_message(1, status);
}