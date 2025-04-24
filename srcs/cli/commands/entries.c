#include "kv_store.h"
#include "kv_table.h"
#include "libs.h"
#include "cli.h"

static int	populate_row_data(char **row_data, t_kv_pair *pair)
{
	row_data[0] = strdup(pair->key);
	if (!row_data[0])
	{
		log_message(1, ERROR_MEMORY_ALLOCATION_CODE);
		return (-1);
	}
	row_data[1] = strdup(pair->value);
	if (!row_data[1])
	{
		log_message(1, ERROR_MEMORY_ALLOCATION_CODE);
		free(row_data[0]);
		return (-1);
	}
	row_data[2] = NULL;
	return (0);
}

static char	***collect_table_data(t_kv_table *table, int *row_count)
{
	int			i;
	int			row;
	char		***data;
	t_kv_pair	*current;

	i = 0;
	row = 0;
	data = malloc(sizeof(char **) * (table->size + 1));
	if (!data)
	{
		log_message(1, ERROR_MEMORY_ALLOCATION_CODE);
		return (NULL);
	}
	while (i < table->capacity)
	{
		current = table->buckets[i];
		while (current)
		{
			data[row] = malloc(sizeof(char *) * 3);
			if (!data[row])
			{
				log_message(1, ERROR_MEMORY_ALLOCATION_CODE);
                cleanup_data_table(data, row);
                return NULL;
			}
			if (populate_row_data(data[row], current) < 0)
			{
				cleanup_data_table(data, row);
				return (NULL);
			}
			row++;
			current = current->next;
		}
		i++;
	}
	data[row] = NULL;
	*row_count = row;
	return (data);
}

void	cmd_entries(t_kv_store *store, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	int				rows;
	char			***data;
	t_data_table	data_table;
	const char		*column_titles[] = {"key", "value", "test", NULL};
	
	rows = 0;
	init_data_table(&data_table, store->name, column_titles);
	data = collect_table_data(store->table, &rows);
	if (!data)
	{
		data_table.data = NULL;
	}
	else 
	{
		data_table.data = data;
	}
	display_table(data_table);
	if (data) cleanup_data_table(data, rows);
}