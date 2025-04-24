#include "kv_store.h"
#include "kv_table.h"
#include "cli.h"

void	handle_set(t_kv_store *store, int argc, char **argv)
{
	t_status_code	status;

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
	status = kv_set(store->table, argv[1], argv[2]);
	log_message(1, status);
}

void	handle_get(t_kv_store *store, int argc, char **argv)
{
	t_status_code	status;
	const char		*value;

	if (argc != 2)
	{
		logger(2, "Usage: get <key>");
		return;
	}
	status = kv_get(store->table, argv[1], &value);
	if (status == SUCCESS_CODE && value)
		logger(2, "%s", (char *)value);
	else
		log_message(1, status);
}

void	handle_delete(t_kv_store *store, int argc, char **argv)
{
	t_status_code	status;

	if (argc != 2)
	{
		logger(2, "Usage: delete <key>");
		return;
	}
	status = kv_delete(store->table, argv[1]);
	log_message(1, status);
}

void 	handle_save(t_kv_store *store, int argc, char **argv)
{
	t_status_code	status;

	if (argc != 2)
	{
		logger(2, "Usage: save <filename>");
		return ;
	}
	status = kv_save_file(store->table, argv[1]);
	log_message(1, status);
}

void    handle_exit(t_kv_store *store, int argc, char **argv)
{
    (void)argc;
    (void)argv;
    t_status_code   status;
    status = kv_save_file(store->table, strcat(store->name, ".kvdb"));
    kv_free_table(store->table);
    store->table = NULL;
    store->name[0] = '\0';
    log_message(1, status);
}

void	kv_table_commands(void)
{
	size_t				i;
	static t_command 	commands[] = {
		{
			.name = "set",
			.usage = "set <key> <value>",
			.description = "",
			.handler = handle_set,
			.flags = TABLE_REQUIRED
		},
		{
			.name = "get",
			.usage = "get <key>",
			.description = "",
			.handler = handle_get,
			.flags = TABLE_REQUIRED
		},
		{
			.name = "delete",
			.usage = "delete <key>",
			.description = "",
			.handler = handle_delete,
			.flags = TABLE_REQUIRED
		},
		{
			.name = "entries",
			.usage = "entries",
			.description = "",
			.handler = handle_entries,
			.flags = TABLE_REQUIRED
		},
		{
			.name = "save",
			.usage = "save <key>",
			.description = "",
			.handler = handle_save,
			.flags = TABLE_REQUIRED
		},
		{
			.name = "exit",
			.usage = "exit",
			.description = "",
			.handler = handle_exit,
			.flags = TABLE_REQUIRED
		}
	};

	i = 0;
	while (i < sizeof(commands) / sizeof(commands[i]))
	{
		register_command(&commands[i]);
		i++;
	}
}