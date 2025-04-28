#include "kv_table.h"
#include "error.h"
#include "libs.h"

unsigned int hash(const char *key, size_t capacity)
{
	int i = 0;
	int hash = 5381;

	while (key[i])
	{
		hash = (hash * 33) + key[i];
		i++;
	}
	return ((hash % capacity + capacity) % capacity);
}

t_status_code	kv_set(t_kv_table *table, const char *key, void *value, size_t value_size, t_kv_type type)
{
	unsigned int	index;
	t_kv_pair		*new_pair;
	t_kv_pair		*current;
	t_status_code	status;

	if (table->size >= table->capacity * 0.75)
	{
		status = kv_resize(table);
		if (status != SUCCESS_CODE)
			return (status);
	}
	index = hash(key, table->capacity);
	current = table->buckets[index];
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			// strncpy(current->value, value, sizeof(current->value) - 1);
			// current->value[sizeof(current->value) - 1] = '\0';
			free(current->value);
			current->value = malloc(sizeof(char) * (value_size + 1));
			if (!current->value)
				return (ERROR_MEMORY_REALLOCATION_CODE);
			memcpy(current->value, value, value_size);
			((char *)current->value)[value_size] = '\0';
			current->value_size = value_size;
			current->type = type;
			return (WARNING_KEY_EXISTS_CODE);
		}
		current = current->next;
	}
	new_pair = malloc(sizeof(t_kv_pair));
	if (!new_pair)
	{
		return (ERROR_MEMORY_ALLOCATION_CODE);
	}
	// strncpy(new_pair->key, key, sizeof(new_pair->key) - 1);
	// strncpy(new_pair->value, value, sizeof(new_pair->value) - 1);
	// new_pair->key[sizeof(new_pair->key) - 1] = '\0';
	// new_pair->value[sizeof(new_pair->value) - 1] = '\0';
	strncpy(new_pair->key, key, sizeof(new_pair->key) - 1);
	new_pair->key[sizeof(new_pair->key) - 1] = '\0';
	new_pair->value = malloc(sizeof(char) * (value_size + 1));
	if (!new_pair->value)
	{
		free(new_pair);
		return (ERROR_MEMORY_ALLOCATION_CODE);
	}
	memcpy(new_pair->value, value, value_size);
	((char *)new_pair->value)[value_size] = '\0';
	new_pair->value_size = value_size;
	new_pair->type = type;
	new_pair->next = table->buckets[index];
	table->buckets[index] = new_pair;
	table->size++;
	return (SUCCESS_CODE);
}

t_status_code	kv_get(t_kv_table *table, const char *key, void **output, t_kv_type type)
{
	unsigned int index;
	t_kv_pair *current;

	index = hash(key, table->capacity);
	if (table->buckets[index] == NULL)
		return (ERROR_KEY_NOT_FOUND_CODE);
	current = table->buckets[index];
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			if (current->type == type)
			{
				*output = current->value;
				return (SUCCESS_CODE);
			}
			else
				return (ERROR_VALUE_TYPE_MISMATCH_CODE);
		}
		current = current->next;
	}
	return (ERROR_KEY_NOT_FOUND_CODE);
};

t_status_code	kv_delete(t_kv_table *table, const char *key)
{
	unsigned int index;
	t_kv_pair *current;
	t_kv_pair *previous;

	index = hash(key, table->capacity);
	if (table->buckets[index] == NULL)
		return (ERROR_KEY_NOT_FOUND_CODE);
	current = table->buckets[index];
	previous = NULL;
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			if (previous == NULL)
				table->buckets[index] = current->next;
			else
				previous = current->next;
			free(current);
			return (SUCCESS_CODE);
		}
		previous = current;
		current = current->next;
	}
	return (ERROR_KEY_NOT_FOUND_CODE);
};

t_status_code	kv_init_table(t_kv_table **table, int capacity)
{
    *table = malloc(sizeof(t_kv_table));
    if (!*table)
        return (ERROR_MEMORY_ALLOCATION_CODE);
    (*table)->capacity = capacity;
    (*table)->size = 0;
    (*table)->buckets = calloc(capacity, sizeof(t_kv_pair *));
    if (!(*table)->buckets)
    {
        free(*table);
		*table = NULL;
        return (ERROR_INIT_TABLE_CODE);
    }
    return (SUCCESS_CODE);
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
        node = old_buckets[i];
        while (node)
        {
            result = kv_set(table, node->key, node->value, node->value_size, node->type);
            if (result != SUCCESS_CODE)
            {
                revert_resize(table, old_buckets, old_capacity);
                return (result);
            }
            tmp = node;
            node = node->next;
			free(tmp->value);
            free(tmp);
        }
        i++;
    }
    free(old_buckets);
    return (SUCCESS_CODE);
}