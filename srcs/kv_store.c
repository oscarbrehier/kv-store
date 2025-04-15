#include "kv_store.h"

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

void	kv_set(t_kv_table *table, const char *key, const char *value)
{
	unsigned	int index;
	t_kv_pair	*new_pair;
	t_kv_pair	*current;

	if (table->size >= table->capacity * 0.75)
		kv_resize(table);
	index = hash(key, table->capacity);
	current = table->buckets[index];
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			strncpy(current->value, value, sizeof(current->value) - 1);
			current->value[sizeof(current->value) - 1] = '\0';
			logger(1, ERROR_KEY_EXISTS);
			return ;
		}
		current = current->next;
	}
	new_pair = malloc(sizeof(t_kv_pair));
	if (!new_pair)
	{
		logger(1, ERROR_MEMORY_ALLOCATION);
		return ;
	}
	strncpy(new_pair->key, key, sizeof(new_pair->key) - 1);
	strncpy(new_pair->value, value, sizeof(new_pair->value) - 1);
	new_pair->key[sizeof(new_pair->key) - 1] = '\0';
	new_pair->value[sizeof(new_pair->value) - 1] = '\0';
	new_pair->next = table->buckets[index];
	table->buckets[index] = new_pair;
	table->size++;
	logger(2, SUCCESS);
}

const char *kv_get(t_kv_table *table, const char *key)
{
	unsigned int index;
	t_kv_pair *current;

	index = hash(key, table->capacity);
	if (table->buckets[index] == NULL)
	{
		logger(1, ERROR_MEMORY_ALLOCATION);
		return (NULL);
	}
	current = table->buckets[index];
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	logger(1, ERROR_KEY_NOT_FOUND);
	return (NULL);
};

void	kv_delete(t_kv_table *table, const char *key)
{
	unsigned int index;
	t_kv_pair *current;
	t_kv_pair *previous;

	index = hash(key, table->capacity);
	if (table->buckets[index] == NULL)
	{
		logger(1, ERROR_KEY_NOT_FOUND);
		return ;
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
				previous = current->next;
			free(current);
			logger(2,SUCCESS);
			return ;
		}
		previous = current;
		current = current->next;
	}
	logger(1, ERROR_KEY_NOT_FOUND);
};

void	kv_list(t_kv_pair **table)
{
	int	i;
	t_kv_pair *current;

	i = 0;
	
	while (i < TABLE_SIZE)
	{
		current = table[i];
		while (current != NULL)
		{
			printf("%s:%s\n", current->key, current->value);
			current = current->next;
		}
		i++;
	}
}

void	print_header(int key_width, int value_width)
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

void	print_table(t_kv_table *table)
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
	print_header((int)key_width, (int)value_width);
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