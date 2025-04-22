#include "kv_store.h"
#include "libs.h"
#include "cli.h"

static int	populate_row_data(char **row_data, t_kv_pair *pair)
{
	row_data[0] = strdup(pair->key);
	if (!row_data[0])
	{
		logger(1, ERROR_MEMORY_ALLOCATION);
		return (-1);
	}
	row_data[1] = strdup(pair->value);
	if (!row_data[1])
	{
		logger(1, ERROR_MEMORY_ALLOCATION);
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
		logger(1, ERROR_MEMORY_ALLOCATION);
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
				logger(1, ERROR_MEMORY_ALLOCATION);
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

void	cmd_entries(t_kv_table *table, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	int				rows;
	char			***data;
	t_data_table	data_table;
	const char		*column_titles[] = {"key", "value", "test", NULL};
	
	rows = 0;
	init_data_table(&data_table, "Key-Value Store", column_titles);
	data = collect_table_data(table, &rows);
	if (!data)
		return ;
	data_table.data = data;
	display_table(data_table);
	cleanup_data_table(data, rows);
}