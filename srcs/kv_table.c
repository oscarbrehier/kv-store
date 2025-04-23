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

void    revert_resize(t_kv_table *table, t_kv_pair **old_buckets, int old_capacity)
{
    int         i;
    t_kv_pair   *current;
    t_kv_pair   *next;

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
    table->buckets = old_buckets;
    table->capacity = old_capacity;
}

t_status_code    kv_resize(t_kv_table *table)
{
    int             i;
    int             old_capacity;
    t_kv_pair       **old_buckets;
    t_kv_pair       *node;
    t_kv_pair       *tmp;
    t_status_code    result;

    old_capacity = table->capacity;
    old_buckets = table->buckets;
    table->capacity *= 2;
    table->size = 0;
    table->buckets = calloc(table->capacity, sizeof(t_kv_pair *));
    if (!table->buckets)
    {
        table->buckets = old_buckets;
        table->capacity = old_capacity;
        return (ERROR_MEMORY_ALLOCATION_CODE);
    }
    i = 0;
    while (i < old_capacity)
    {
        node = table->buckets[i];
        while (node)
        {
            result = kv_set(table, node->key, node->value);
            if (result != SUCCESS_CODE)
            {
                revert_resize(table, old_buckets, old_capacity);
                return (result);
            }
            tmp = node;
            node = node->next;
            free(tmp);
        }
        i++;
    }
    free(old_buckets);
    return (SUCCESS_CODE);
}