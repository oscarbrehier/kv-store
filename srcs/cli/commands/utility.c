#include "kv_store.h"
#include "cli.h"

void	handle_clear(t_kv_store *store, int argc, char **argv)
{
	(void)store;
	(void)argc;
	(void)argv;
	clear_console();
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