#include "kv_store.h"
#include "kv_table.h"
#include "libs.h"
#include "cli.h"

t_kv_table *command_table = NULL;

int	init_command_table(void)
{
	kv_init_table(&command_table, 32);
	if (!command_table)
		return (1);
	return (0);
}

void cleanup_command_sys(void)
{
    if (command_table) {
        kv_free_table(command_table);
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
	return kv_set(command_table, cmd->name, cmd, sizeof(t_command), COMMAND);
}

t_command *find_command(const char *name)
{
	void	*value;

    if (!command_table || !name)
    {
		return (NULL);
	}
	value = NULL;
	if (kv_get(command_table, name, &value, COMMAND) != SUCCESS_CODE)
		return (NULL);
    return (t_command *)value;
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