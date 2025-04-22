#include "kv_store.h"
#include "libs.h"
#include "cli.h"

void	handle_set(t_kv_table *table, int argc, char **argv)
{
	if (argc != 3)
	{
		logger(2, "Usage: set <key> <value>");
		return;
	}
	if (!is_valid_key(argv[1]))
	{
		logger(2, "Error: Key contains illegal characters");
		return ;
	}
	kv_set(table, argv[1], argv[2]);
}

void	handle_get(t_kv_table *table, int argc, char **argv)
{
	const char	*res;

	if (argc != 2)
	{
		logger(2, "Usage: get <key>\n");
		return;
	}
	res = kv_get(table, argv[1]);
	if (res != NULL)
		printf("%s\n", res);
}

void	handle_delete(t_kv_table *table, int argc, char **argv)
{
	if (argc != 2)
	{
		logger(2, "Usage: delete <key>");
		return;
	}
	kv_delete(table, argv[1]);
}

void	handle_list(t_kv_table *table, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	kv_render_table(table);
}

void 	handle_save(t_kv_table *table, int argc, char **argv)
{
	if (argc != 2)
	{
		logger(2, "Usage: save <filename>");
		return ;
	}
	kv_save_file(table, argv[1]);
}

void	handle_load(t_kv_table *table, int argc, char **argv)
{
	if (argc != 2)
	{
		logger(2, "Usage: save <filename>");
		return ;
	}
	kv_load_file(table, argv[1]);
}

void	exec_cmd(t_kv_table *table, int argc, char **argv)
{
	long unsigned int	i;
	t_command commands[] = {
		{"set", handle_set},
		{"get", handle_get},
		{"delete", handle_delete},
		{"save", handle_save},
		{"load", handle_load},
		{"entries", cmd_entries},
		{"list", cmd_list},
	};

	i = 0;
	while (i < sizeof(commands) / sizeof(commands[0]))
	{
		if (ft_strcmp(argv[0], commands[i].cmd) == 0)
		{
			commands[i].handler(table, argc, argv);
			return;
		}
		i++;
	}
}