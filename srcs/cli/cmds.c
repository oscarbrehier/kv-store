#include "kv_store.h"

void	handle_set(t_kv_pair **table, int argc, char **argv)
{
	if (argc != 3)
	{
		fn_putstr("Usage: set <key> <value>\n");
		return;
	}
	kv_set(table, argv[1], argv[2]);
}

void	handle_get(t_kv_pair **table, int argc, char **argv)
{
	const char	*res;

	if (argc != 2)
	{
		fn_putstr("Usage: get <key>\n");
		return;
	}
	res = kv_get(table, argv[1]);
	if (res != NULL)
		printf("%s\n", res);
}

void	handle_delete(t_kv_pair **table, int argc, char **argv)
{
	if (argc != 2)
	{
		fn_putstr("Usage: delete <key>\n");
		return;
	}
	kv_delete(table, argv[1]);
}

void	handle_list(t_kv_pair **table, int argc, char **argv)
{
	(void)argc;
	(void)argv;
	print_table(table);
}

void 	handle_save(t_kv_pair **table, int argc, char **argv)
{
	if (argc != 2)
	{
		fn_putstr("Usage: save <filename>\n");
		return ;
	}
	kv_save_file(table, argv[1]);
}

void	handle_load(t_kv_pair **table, int argc, char **argv)
{
	if (argc != 2)
	{
		fn_putstr("Usage: save <filename>\n");
		return ;
	}
	kv_load_file(table, argv[1]);
}

void	exec_cmd(t_kv_pair **table, int argc, char **argv)
{
	long unsigned int	i;
	t_command commands[] = {
		{"set", handle_set},
		{"get", handle_get},
		{"delete", handle_delete},
		{"list", handle_list},
		{"save", handle_save},
		{"load", handle_load}
	};

	i = 0;
	while (i < sizeof(commands) / sizeof(commands[0]))
	{
		if (fn_strcmp(argv[0], commands[i].cmd) == 0)
		{
			commands[i].handler(table, argc, argv);
			return;
		}
		i++;
	}
}