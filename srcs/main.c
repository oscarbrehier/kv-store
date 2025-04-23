#include "kv_store.h"
#include "libs.h"

void kv_free_table(t_kv_table *table)
{
    int     i;
    t_kv_pair   *current;
    t_kv_pair   *next;
    if (!table)
        return;
    i = 0;
    while (i < table->capacity)
    {
        current = table->buckets[i];
        while (current)
        {
            next = current->next;
            free(current);
            current = next;
        }
        i++;
    }
    free(table->buckets);
    free(table);
}

int	main(void)
{
    t_kv_store      *store;
    t_kv_table      *table;
    t_status_code   ft_res;

    store = (t_kv_store *)malloc(sizeof(t_kv_store));
    if (!store)
    {
        logger(1, ERROR_MEMORY_ALLOCATION);
        return (1);
    }

    table = kv_init_table(8);
    if (!table)
    {
        logger(1, "Error: Failed to initialize table");
        return (1);
    }
    if ((ft_res = kv_load_file(table, "./data/data.kvdb")) != SUCCESS_CODE)
    {
        log_message(1, ft_res);
        return (1);
    }
    store->table = table;
    strcpy(store->name, "data");
    run_cli(store);
	kv_free_table(store->table);
    free(store);
	return (0);
};