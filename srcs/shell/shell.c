#include "kv_store.h"
#include "kv_table.h"
#include "libs.h"
#include "shell.h"
#include "utils.h"

void	read_input(char **input)
{
	int		c;
	int 	capacity;
	int		length;
	char	*temp;

	capacity = 1;
	length = 0;
	*input = malloc(sizeof(char) * capacity);
	if (!*input)
		return ;
	while ((c = getchar()) != '\n' && c != EOF)
	{
		if (length + 1 >= capacity)
		{
			capacity *= 2;
			temp = realloc(*input, capacity);
			if (!temp)
			{
				free(*input);
				return ;
			}
			*input = temp;
		}
		(*input)[length++] = (char)c;
	}
	if (length == 0)
	{
		(*input)[0] = '\0';
	}
	else
	{
		temp = realloc(*input, length + 1);
		if (temp)
			*input = temp;
		(*input)[length] = '\0';
	}
}

void	parse_input(char *input, int *argc, char ***argv)
{
	int i;
	int j;
	int k;
	int arg_l;
	int arg_i;

	i = 0;
	arg_l = 0;
	arg_i = 0;
	*argc = count_words(input);
	*argv = malloc(sizeof(char *) * (*argc + 1));
	if (!*argv)
		return ;
	while (input[i] && arg_i < *argc)
	{
		if (is_space(input[i]))
		{
			i++;
			continue;
		}
		
		arg_l = 0;
		while (input[i] && !is_space(input[i]))
		{
			arg_l++;
			i++;
		}
		(*argv)[arg_i] = malloc(sizeof(char) * (arg_l + 1));
		if (!(*argv)[arg_i])
		{
			k = 0;
			while (k < arg_i)
				free((*argv)[k++]);
			free(*argv);
			*argv = NULL;
			return ;
		}
		j = 0;
		while (j < arg_l)
		{
			(*argv)[arg_i][j] = input[i - arg_l + j];
			j++;
		}
		(*argv)[arg_i][j] = '\0';
		arg_i++;	
	}
	(*argv)[arg_i] = NULL;
}

void	pprompt(const char *table_name)
{
	if (table_name[0] != '\0')
		printf(BRIGHT_BLUE BOLD "kv:(" RESET GREEN BOLD "%s" BRIGHT_BLUE ") " RESET, table_name);
	else
		printf(RESET BRIGHT_BLUE BOLD "kv: " RESET);
}

void	clear_console(void)
{
	#ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int	init_shell(void)
{
	clear_console();
	if (init_command_table() != 0)
		goto error;

	// Register commands
	if (kv_store_commands() != 0)
		goto error;
	if (kv_table_commands() != 0)
		goto error;
	if (kv_string_ops_commands() != 0)
		goto error;
	if (utility_commands() != 0)
		goto error;
	return (0);
error:
	logger(1, "Error: Failed to initialize SHELL");
	return (1);
}

void	destroy_shell(void)
{
	cleanup_command_sys();
}

void	free_argv(char **argv)
{
	int	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}

void	run_shell(t_kv_store *store)
{
	char		*input;
	char		**argv;
	int			argc;

	input = NULL;
	argv = NULL;
	argc = 0;
	while (1)
	{
		pprompt(store->name);
		read_input(&input);
		if (!input)
		{
			logger(1, "Error: Failed to read user input");
			continue ;
		}
		parse_input(input, &argc, &argv);
		if (argc == 0)
		{
			free(input);
			free_argv(argv);
			input = NULL;
			argv = NULL;
			continue ;
		}
		if (ft_strcmp(argv[0], "quit") == 0)
		{
			free(input);
			free_argv(argv);
			destroy_shell();
			break ;
		}
		exec_cmd(store, argc, argv);
		free(input);
		free_argv(argv);
		input = NULL;
		argv = NULL;
	}
}