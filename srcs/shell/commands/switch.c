#include "kv_table.h"
#include "kv_store.h"
#include "libs.h"

void    handle_switch(t_kv_store *store, int argc, char **argv)
{
    const char		*file_path;
    t_kv_table		*temp;
    t_status	status;

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
    if (status.code != SUCCESS)
    {
        status_log(1, status.code);
        free((void *)file_path);
        kv_free_table(store->table);
        free(store);
        return ;
    }
    if (store->table)
        free(store->table);
    store->table = temp;
    strcpy(store->name, argv[1]);
    status_log(2, SUCCESS);
    free((void *)file_path);
}