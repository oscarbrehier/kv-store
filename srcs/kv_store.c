#include "kv_store.h"

unsigned int hash(const char *key)
{
	int i = 0;
	int hash = 5381;

	while (key[i])
	{
		hash = (hash * 33) + key[i];
		i++;
	}
	return ((hash % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE);
}

void kv_set(t_kv_pair **table, const char *key, const char *value)
{
	unsigned int index;
	t_kv_pair *new_pair;
	t_kv_pair *current;

	index = hash(key);
	current = table[index];
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			strncpy(current->value, value, sizeof(current->value) - 1);
			current->value[sizeof(current->value) - 1] = '\0';
			return;
		}
		current = current->next;
	}
	new_pair = malloc(sizeof(t_kv_pair));
	if (!new_pair)
		return;
	strncpy(new_pair->key, key, sizeof(new_pair->key) - 1);
	strncpy(new_pair->value, value, sizeof(new_pair->value) - 1);
	new_pair->key[sizeof(new_pair->key) - 1] = '\0';
	new_pair->value[sizeof(new_pair->value) - 1] = '\0';
	new_pair->next = table[index];
	table[index] = new_pair;
}

const char *kv_get(t_kv_pair **table, const char *key)
{
	unsigned int index;
	t_kv_pair *current;

	index = hash(key);
	if (table[index] == NULL)
		return (NULL);
	current = table[index];
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
};

void kv_delete(t_kv_pair **table, const char *key)
{
	unsigned int index;
	t_kv_pair *current;
	t_kv_pair *previous;

	index = hash(key);
	if (table[index] == NULL)
		return;
	current = table[index];
	previous = NULL;
	while (current != NULL)
	{
		if (strcmp(current->key, key) == 0)
		{
			if (previous == NULL)
				table[index] = current->next;
			else
				previous = current->next;

			free(current);
			return;
		}
		previous = current;
		current = current->next;
	}
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