#include "kv_store.h"

t_kv_table *kv_init_table(int capacity)
{
    t_kv_table *table;

    table = malloc(sizeof(t_kv_table));
    if (!table)
        return (NULL);
    table->capacity = capacity;
    table->size = 0;
    table->buckets = calloc(capacity, sizeof(t_kv_pair *));
    if (!table->buckets)
    {
        free(table);
        return (NULL);
    }
    return (table);
}

void    kv_resize(t_kv_table *table)
{
    int         i;
    int         old_capacity;
    t_kv_pair   **old_buckets;
    t_kv_pair   *node;
    t_kv_pair   *tmp;

    old_capacity = table->capacity;
    old_buckets = table->buckets;
    table->capacity *= 2;
    table->size = 0;
    table->buckets = calloc(table->capacity, sizeof(t_kv_pair *));
    if (!table->buckets)
    {
        table->buckets = old_buckets;
        table->capacity = old_capacity;
        return ;
    }
    i = 0;
    while (i < old_capacity)
    {
        node = table->buckets[i];
        while (node)
        {
            kv_set(table, node->key, node->value);
            tmp = node;
            node = node->next;
            free(tmp);
        }
        i++;
    }
    free(old_buckets);
}