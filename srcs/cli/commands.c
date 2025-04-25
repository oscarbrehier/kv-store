#include "kv_store.h"
#include "kv_table.h"
#include "libs.h"
#include "cli.h"
#include "hashtable.h"

t_hashtable *command_table = NULL;

int	init_command_table(void)
{
	command_table = ht_create(64);
	if (!command_table)
		return (1);
	return (0);
}

void cleanup_command_sys(void)
{
    if (command_table) {
        ht_destroy(command_table);
        command_table = NULL;
    }
}

t_status_code	register_command(t_command *cmd)
{
    if (!command_table)
        return (ERROR_COMMAND_TABLE_MISSING_CODE);
	else if (!cmd)
		return (ERROR_COMMAND_STRUCT_NOT_FOUND_CODE);
	else if (!cmd->name)
		return (ERROR_COMMAND_NAME_MISSING_CODE);
    ht_set(command_table, cmd->name, cmd);
	return (SUCCESS_CODE);
}

t_command *find_command(const char *name)
{
    if (!command_table || !name)
        return NULL;
        
    return ht_get(command_table, name);
}

void	exec_cmd(t_kv_store *store, int argc, char **argv)
{
	t_command	*cmd = NULL;

	if (argc < 1 || !argv || !argv[0])
		return ;
	cmd = find_command(argv[0]);
	if (!cmd)
	{
		logger(1, "%s: command not found", argv[0]);
		return ;
	}
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