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
    t_kv_table *table;

    table = kv_init_table(8);
    if (!table)
    {
        logger(1, "Error: Failed to initialize table");
        return (1);
    }
	run_cli(table);
	kv_free_table(table);
	return (0);
};