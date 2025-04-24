#include "kv_store.h"
#include "kv_table.h"
#include "libs.h"
#include "cli.h"
#include "hashtable.h"

static t_hashtable *command_table = NULL;

void init_command_sys(void)
{
    command_table = ht_create(64);
}

void cleanup_command_sys(void)
{
    if (command_table) {
        ht_destroy(command_table);
        command_table = NULL;
    }
}

void register_command(t_command *cmd)
{
    if (!command_table || !cmd || !cmd->name)
        return;
    
    ht_set(command_table, cmd->name, cmd);
}

t_command *find_command(const char *name)
{
    if (!command_table || !name)
        return NULL;
        
    return ht_get(command_table, name);
}

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

void	handle_load(t_kv_store *store, int argc, char **argv)
{
	t_status_code	status;

	if (argc != 2)
	{
		logger(2, "Usage: save <filename>");
		return ;
	}
	status = kv_load_file(store->table, argv[1]);
	log_message(1, status);
}

void	cmd_clear(t_kv_store *store, int argc, char **argv)
{
	(void)store;
	(void)argc;
	(void)argv;
	clear_console();
}

void	register_all_commads(void)
{
	init_command_sys();

	register_set_command();
}

void	exec_cmd(t_kv_store *store, int argc, char **argv)
{
	t_command	*cmd = NULL;

	if (argc < 1 || !argv || !argv[0])
		return ;
	cmd = find_command(argv[0]);
	if (!cmd)
		return ;
	if ((cmd->flags & TABLE_REQUIRED) && !store->table)
	{
		logger(1, "Error: No store has been selected.");
		return ;
	}
	if ((cmd->flags & NO_TABLE_ALLOWED) && store->table)
	{
		logger(1, "Error: You must exit table `%s` first.", store->name);
		return ;
	}
	cmd->handler(store, argc, argv);
}

// void	exec_cmd(t_kv_store *store, int argc, char **argv)
// {
// 	long unsigned int	i;
// 	t_command commands[] = {
// 		{"set", handle_set, TABLE_REQUIRED},
// 		{"get", handle_get, TABLE_REQUIRED},
// 		{"delete", handle_delete, TABLE_REQUIRED},
// 		{"save", handle_save, TABLE_REQUIRED},
// 		{"load", handle_load, TABLE_REQUIRED},
// 		{"entries", cmd_entries, TABLE_REQUIRED},
// 		{"exit", cmd_exit, TABLE_REQUIRED},
// 		{"list", cmd_list, NO_TABLE_ALLOWED},
// 		{"switch", cmd_switch, NO_TABLE_ALLOWED},
// 		{"create", cmd_create, NO_TABLE_ALLOWED},
// 		{"drop", cmd_drop, NO_TABLE_ALLOWED},
// 		{"rename", cmd_rename, NO_TABLE_ALLOWED},
// 		{"clear", cmd_clear, 0}
// 	};

// 	i = 0;
// 	while (i < sizeof(commands) / sizeof(commands[0]))
// 	{
// 		if (ft_strcmp(argv[0], commands[i].cmd) == 0)
// 		{
// 			if ((commands[i].flags & TABLE_REQUIRED) && !store->table)
// 			{
// 				logger(1, "Error: No store has been selected.");
// 				return ;
// 			}
// 			if ((commands[i].flags & NO_TABLE_ALLOWED) && store->table)
// 			{
// 				logger(1, "Error: You must exit table `%s` first.", store->name);
// 				return ;
// 			}
// 			commands[i].handler(store, argc, argv);
// 			return;
// 		}
// 		i++;
// 	}
// }