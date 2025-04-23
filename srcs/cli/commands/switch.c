#include "kv_table.h"
#include "kv_store.h"
#include "libs.h"

void    cmd_switch(t_kv_store *store, int argc, char **argv)
{
    char            *file_path;
    t_kv_table      *temp;
    t_status_code   ft_res;

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
    file_path = construct_table_path(argv[1], "./data/");
    if (!file_path)
    {
        free(file_path);
        return ;
    }
    ft_res = kv_load_file(temp, file_path);
    if (ft_res != SUCCESS_CODE)
    {
        log_message(1, ft_res);
        free(file_path);
        kv_free_table(store->table);
        free(store);
        return ;
    }
    if (store->table)
        free(store->table);
    store->table = temp;
    strcpy(store->name, argv[1]);
    log_message(2, SUCCESS_CODE);
    free(file_path);
}