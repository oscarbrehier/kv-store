#include "kv_store.h"
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

t_status_code	kv_set(t_kv_table *table, const char *key, const char *value)
{
	unsigned int	index;
	t_kv_pair		*new_pair;
	t_kv_pair		*current;
	t_status_code	ft_res;

	if (table->size >= table->capacity * 0.75)
	{
		ft_res = kv_resize(table);
		if (ft_res != SUCCESS_CODE)
			return (ft_res);
	}
	index = hash(key, table->capacity);
	current = table->buckets[index];
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			strncpy(current->value, value, sizeof(current->value) - 1);
			current->value[sizeof(current->value) - 1] = '\0';
			return (WARNING_KEY_EXISTS_CODE);
		}
		current = current->next;
	}
	new_pair = malloc(sizeof(t_kv_pair));
	if (!new_pair)
	{
		return (ERROR_MEMORY_ALLOCATION_CODE);
	}
	strncpy(new_pair->key, key, sizeof(new_pair->key) - 1);
	strncpy(new_pair->value, value, sizeof(new_pair->value) - 1);
	new_pair->key[sizeof(new_pair->key) - 1] = '\0';
	new_pair->value[sizeof(new_pair->value) - 1] = '\0';
	new_pair->next = table->buckets[index];
	table->buckets[index] = new_pair;
	table->size++;
	return (SUCCESS_CODE);
}

t_status_code	kv_get(t_kv_table *table, const char *key, const char **output)
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
			*output = current->value;
			return (SUCCESS_CODE);
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

void	kv_render_table_header(int key_width, int value_width)
{
	int i;
	
	printf("\n");
    printf("  %-*s   | %-*s  \n", key_width, "Key", value_width, "Value");
	printf("  ");
    i = 0;
	while (i < key_width + 2)
	{
		printf("-");
		i++;
	}
    printf("-+-");
	i = 0;
    while (i < value_width + 2)
	{
		printf("-");
		i++;
	}
    printf("\n");
}

void	kv_render_table(t_kv_table *table)
{
    int			i;
	int 		count;
    size_t		key_width;
    size_t		value_width;
    t_kv_pair	*current;
    
	i = 0;
	key_width = 10;
	value_width = 10;
	while (i < table->capacity)
	{
		current = table->buckets[i];
        while (current)
        {
            if (strlen(current->key) > key_width)
                key_width = strlen(current->key);
            if (strlen(current->value) > value_width)
                value_width = strlen(current->value);
            current = current->next;
        }
		i++;
	}
	kv_render_table_header((int)key_width, (int)value_width);
	count = 0;
	i = 0;
    while (i < table->capacity)
	{
		current = table->buckets[i];
        while (current)
        {
            printf("  %-*s   | %-*s  \n", (int)key_width, current->key, (int)value_width, current->value);
            current = current->next;
            count++;
        }
		i++;
	}
    printf("  (%d rows)\n", count);
	printf("\n");
}