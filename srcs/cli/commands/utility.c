#include "kv_store.h"
#include "cli.h"
#include "hashtable.h"

void	handle_clear(t_kv_store *store, int argc, char **argv)
{
	(void)store;
	(void)argc;
	(void)argv;
	clear_console();
}

void    handle_help(t_kv_store *store, int argc, char **argv)
{
    (void)store;
	(void)argc;
	(void)argv;
	size_t		i;
    t_ht_entry	*current;
    t_ht_entry	*next;
	t_command	*cmd;

	i = 0;
	while (i < command_table->size)
	{
		current = command_table->entries[i];
		while (current)
		{
			next = current->next;
			cmd = current->value;
			printf("%s				%s\n", cmd->name, cmd->usage);
			current = next;	
		}
		i++;
	}
}

int	utility_commands(void)
{
    size_t				i;
	t_status_code		status;
    static t_command	commands[] = {
        {
            .name = "clear",
            .usage = "clear",
            .description = "",
            .handler = handle_clear,
            .flags = NO_FLAG
        },
		{
			.name = "help",
			.usage = "help",
			.description = "",
			.handler = handle_help,
			.flags = NO_FLAG
		}
    };

    i = 0;
    while (i < sizeof(commands) / sizeof(commands[0]))
    {
        status = register_command(&commands[i]);
		if (status != SUCCESS_CODE)
			return (1);
        i++;
    }
	return (0);
}