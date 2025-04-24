#include "kv_store.h"
#include "kv_table.h"

void    cmd_exit(t_kv_store *store, int argc, char **argv)
{
    (void)argc;
    (void)argv;
    t_status_code   status;
    status = kv_save_file(store->table, strcat(store->name, ".kvdb"));
    kv_free_table(store->table);
    store->table = NULL;
    store->name[0] = '\0';
    log_message(1, status);
}