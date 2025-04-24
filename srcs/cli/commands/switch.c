#include "kv_table.h"
#include "kv_store.h"
#include "libs.h"

void    cmd_switch(t_kv_store *store, int argc, char **argv)
{
    const char		*file_path;
    t_kv_table		*temp;
    t_status_code	status;

    kv_init_table(&temp, 8);
    if (!temp)
    {
        logger(1, "Error: failed to init table.");
        return ;
    }
    if (argc != 2)
    {
        logger(2, "Usage: switch <store name>");
        return ;
    }
    construct_table_path(argv[1], "./data/", &file_path);
    if (!file_path)
    {
        free((void *)file_path);
        return ;
    }
    status = kv_load_file(temp, file_path);
    if (status != SUCCESS_CODE)
    {
        log_message(1, status);
        free((void *)file_path);
        kv_free_table(store->table);
        free(store);
        return ;
    }
    if (store->table)
        free(store->table);
    store->table = temp;
    strcpy(store->name, argv[1]);
    log_message(2, SUCCESS_CODE);
    free((void *)file_path);
}