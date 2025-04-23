#include "kv_store.h"
#include "kv_table.h"
#include "libs.h"
#include "cli.h"

void read_input(char **input)
{
	char *temp;
	int c;
	int capacity;
	int length;

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

int	parse_input(char *input, char ***args)
{
	int argc;
	int i;
	int j;
	int k;
	int arg_l;
	int arg_i;

	i = 0;
	arg_l = 0;
	arg_i = 0;
	argc = count_words(input);
	if (argc > 3)
		argc = 3;
	*args = malloc(sizeof(char *) * (argc + 1));
	if (!*args)
		return (0);
	while (input[i] && arg_i < argc)
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
		(*args)[arg_i] = malloc(sizeof(char) * (arg_l + 1));
		if (!(*args)[arg_i])
		{
			k = 0;
			while (k < arg_i)
				free((*args)[k++]);
			free(*args);
			*args = NULL;
			return (0);
		}
		j = 0;
		while (j < arg_l)
		{
			(*args)[arg_i][j] = input[i - arg_l + j];
			j++;
		}
		(*args)[arg_i][j] = '\0';
		arg_i++;	
	}
	(*args)[arg_i] = NULL;
	return (argc);
}

void run_cli(t_kv_store *store)
{
	int		argc;
	char	**argv;
	char	*input;
	int		i;

	input = NULL;
	argc = 0;
	argv = NULL;
	while (1)
	{
		if (ft_strlen(store->name) > 0)
			printf("kv (%s)> ", store->name);
		else
			printf("kv> ");

		read_input(&input);
		if (!input)
		{
			logger(1, "Error");
			continue;
		}
		argc = parse_input(input, &argv);
		if (!argc)
		{
			free(input);
			i = 0;
			while (argv[i])
			{
				free(argv[i]);
				i++;
			}
			free(argv);
			input = NULL;
			argv = NULL;
			continue;
		}
		if (ft_strcmp(argv[0], "quit") == 0)
		{
			free(input);
			i = 0;
			while (argv[i])
			{
				free(argv[i]);
				i++;
			}
			free(argv);
			break;	
		}
		exec_cmd(store, argc, argv);
		free(input);
		i = 0;
		while (argv[i])
		{
			free(argv[i]);
			i++;
		}
		free(argv);
		input = NULL;
		argv = NULL;
	}
}
