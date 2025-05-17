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

t_status	_kv_set_internal(t_kv_table *table, const char *key, void *value, size_t value_size, t_kv_type type, int should_lock)
{
	size_t			alloc_size;
	unsigned int	index;
	t_kv_pair		*new_pair;
	void			*update_value;
	t_kv_pair		*current;
	t_status		status;

	if (should_lock) pthread_rwlock_wrlock(&table->rwlock);
	if (table->size >= table->capacity * 0.75)
	{
		status = kv_resize(table);
		if (status.code != SUCCESS)
		{
			if (should_lock) pthread_rwlock_unlock(&table->rwlock);
			return (status);
		}
	}
	index = hash(key, table->capacity);
	current = table->buckets[index];
	alloc_size = value_size + (type == STRING ? 1 : 0);
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			update_value = malloc(sizeof(char) * alloc_size);
			if (!update_value)
			{
				if (should_lock) pthread_rwlock_unlock(&table->rwlock);
				status = status_create(-1, ERROR_MEMORY_REALLOCATION, LOG_ERROR);
				return (status);
			}
			memcpy(update_value, value, value_size);
			if (type == STRING)
			{
				((char *)update_value)[value_size] = '\0';
			}
			free(current->value);
			current->value = update_value;
			current->value_size = value_size;
			current->type = type;
			if (should_lock) pthread_rwlock_unlock(&table->rwlock);
			status = status_create(-1, WARNING_KEY_EXISTS, LOG_WARNING);
			return (status);
		}
		current = current->next;
	}
	new_pair = malloc(sizeof(t_kv_pair));
	if (!new_pair)
	{
		if (should_lock) pthread_rwlock_unlock(&table->rwlock);
		status = status_create(-1, ERROR_MEMORY_ALLOCATION, LOG_ERROR);
		return (status);
	}
	strncpy(new_pair->key, key, sizeof(new_pair->key) - 1);
	new_pair->key[sizeof(new_pair->key) - 1] = '\0';
	new_pair->value = malloc(sizeof(char) * alloc_size);
	if (!new_pair->value)
	{
		free(new_pair);
		if (should_lock) pthread_rwlock_unlock(&table->rwlock);
		status = status_create(-1, ERROR_MEMORY_ALLOCATION, LOG_ERROR);
		return (status);
	}
	memcpy(new_pair->value, value, value_size);
	if (type == STRING)
	{
		((char *)new_pair->value)[value_size] = '\0';
	}
	new_pair->value_size = value_size;
	new_pair->type = type;
	new_pair->next = table->buckets[index];
	table->buckets[index] = new_pair;
	table->size++;
	if (should_lock) pthread_rwlock_unlock(&table->rwlock);
	status = status_create(0, SUCCESS, LOG_INFO);
	return (status);
}

t_status	kv_set(t_kv_table *table, const char *key, void *value, size_t value_size, t_kv_type type)
{
	return _kv_set_internal(table, key, value, value_size, type, 1);
}

t_status	kv_get(t_kv_table *table, const char *key, void **output, t_kv_type type)
{
	(void)type;
	unsigned int 	index;
	t_kv_pair 		*current;
	t_status		status;

	if (output == NULL)
	{
		return (status_create(-1, ERROR_NULL_POINTER, LOG_ERROR));
	}
	pthread_rwlock_rdlock(&table->rwlock);
	index = hash(key, table->capacity);
	status = status_create(-1, ERROR_KEY_NOT_FOUND, LOG_ERROR);
	if (table->buckets[index] == NULL)
	{
		pthread_rwlock_unlock(&table->rwlock);
		return (status);
	}
	current = table->buckets[index];
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			*output = current->value;
			// if (type == STRING)
			// {
			// 	((char *)*output)[current->value_size] = '\0';
			// }
			status = status_create(0, SUCCESS, LOG_INFO);
			break ; 
		}
		current = current->next;
	}
	pthread_rwlock_unlock(&table->rwlock);
	return (status);
}

t_status	kv_delete(t_kv_table *table, const char *key)
{
	unsigned int 	index;
	t_kv_pair 		*current;
	t_kv_pair 		*previous;
	t_status		status;

	pthread_rwlock_wrlock(&table->rwlock);
	index = hash(key, table->capacity);
	status = status_create(-1, ERROR_KEY_NOT_FOUND, LOG_ERROR);
	if (table->buckets[index] == NULL)
	{
		pthread_rwlock_unlock(&table->rwlock);
		return (status);
	}
	current = table->buckets[index];
	previous = NULL;
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			if (previous == NULL)
				table->buckets[index] = current->next;
			else
				previous->next = current->next;
			free(current->value);
			free(current);
			status = status_create(0, SUCCESS, LOG_INFO);
			break ;
		}
		previous = current;
		current = current->next;
	}
	pthread_rwlock_unlock(&table->rwlock);
	return (status);
};

t_status	kv_init_table(t_kv_table **table, int capacity)
{
	t_status	status;

	*table = malloc(sizeof(t_kv_table));
    if (!*table)
    {
		status = status_create(-1, ERROR_MEMORY_ALLOCATION, LOG_ERROR);
		return (status);
	}
    (*table)->capacity = capacity;
    (*table)->size = 0;
    (*table)->buckets = calloc(capacity, sizeof(t_kv_pair *));
	if (pthread_rwlock_init(&(*table)->rwlock, NULL) != 0)
	{
		free((*table)->buckets);
		free(*table);
		*table = NULL;
		status = status_create(-1, ERROR_INIT_TABLE, LOG_ERROR);
		return (status);
	}
    if (!(*table)->buckets)
    {
        free(*table);
		*table = NULL;
		status = status_create(-1, ERROR_INIT_TABLE, LOG_ERROR);
		return (status);
    }
	status = status_create(0, SUCCESS, LOG_INFO);
	return (status);
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

t_status    kv_resize(t_kv_table *table)
{
    int             i;
    int             old_capacity;
    t_kv_pair       **old_buckets;
    t_kv_pair       *node;
    t_kv_pair       *tmp;
    t_status    	status;

    old_capacity = table->capacity;
    old_buckets = table->buckets;
    table->capacity *= 2;
    table->size = 0;
    table->buckets = calloc(table->capacity, sizeof(t_kv_pair *));
    if (!table->buckets)
    {
        table->buckets = old_buckets;
        table->capacity = old_capacity;
        status = status_create(-1, ERROR_MEMORY_ALLOCATION, LOG_ERROR);
		return (status);
    }
    i = 0;
    while (i < old_capacity)
    {
        node = old_buckets[i];
        while (node)
        {
            status = _kv_set_internal(table, node->key, node->value, node->value_size, node->type, 0);
            if (status.code != SUCCESS)
            {
                revert_resize(table, old_buckets, old_capacity);
                return (status);
            }
            tmp = node;
            node = node->next;
			free(tmp->value);
            free(tmp);
        }
        i++;
    }
    free(old_buckets);
    status = status_create(0, SUCCESS, LOG_INFO);
	return (status);
}

void kv_free_table(t_kv_table *table)
{
    int     i;
    t_kv_pair   *current;
    t_kv_pair   *next;

    if (!table) return;
	pthread_rwlock_wrlock(&table->rwlock);
    i = 0;
    while (i < table->capacity)
    {
        current = table->buckets[i];
        while (current)
        {
            next = current->next;
            free(current->value);
            free(current);
            current = next;
        }
        i++;
    }
	pthread_rwlock_unlock(&table->rwlock);
	pthread_rwlock_destroy(&table->rwlock);
    free(table->buckets);
    free(table);
}