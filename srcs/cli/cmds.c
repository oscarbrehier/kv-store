#include "kv_store.h"
#include "libs.h"
#include "cli.h"

void	handle_set(t_kv_store *store, int argc, char **argv)
{
	t_status_code	result;

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
	result = kv_set(store->table, argv[1], argv[2]);
	log_message(1, result);
}

void	handle_get(t_kv_store *store, int argc, char **argv)
{
	const char	*res;

	if (argc != 2)
	{
		logger(2, "Usage: get <key>");
		return;
	}
	res = kv_get(store->table, argv[1]);
	if (res != NULL)
		printf("%s\n", res);
}

void	handle_delete(t_kv_store *store, int argc, char **argv)
{
	if (argc != 2)
	{
		logger(2, "Usage: delete <key>");
		return;
	}
	kv_delete(store->table, argv[1]);
}

void	handle_list(t_kv_store *store, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	kv_render_table(store->table);
}

void 	handle_save(t_kv_store *store, int argc, char **argv)
{
	if (argc != 2)
	{
		logger(2, "Usage: save <filename>");
		return ;
	}
	kv_save_file(store->table, argv[1]);
}

void	handle_load(t_kv_store *store, int argc, char **argv)
{
	if (argc != 2)
	{
		logger(2, "Usage: save <filename>");
		return ;
	}
	kv_load_file(store->table, argv[1]);
}

void	exec_cmd(t_kv_store *store, int argc, char **argv)
{
	long unsigned int	i;
	t_command commands[] = {
		{"set", handle_set, 1},
		{"get", handle_get, 1},
		{"delete", handle_delete, 1},
		{"save", handle_save, 1},
		{"load", handle_load, 1},
		{"entries", cmd_entries, 1},
		{"list", cmd_list, 0},
		{"exit", cmd_exit, 1},
		{"switch", cmd_switch, 0},
	};

	i = 0;
	while (i < sizeof(commands) / sizeof(commands[0]))
	{
		if (ft_strcmp(argv[0], commands[i].cmd) == 0)
		{
			if (!store->table && commands[i].table_required)
			{
				logger(1, "Error: No store has been selected");
				return ;
			}
			commands[i].handler(store, argc, argv);
			return;
		}
		i++;
	}
}