#include "kv_store.h"
#include "kv_error.h"

const char *error_messages[] = {
    "OK.",
    "Error: Key already exists. Value will be updated.",
    "Error: Failed to allocate memory for key-value pair.",
    "Error: Invalid key or value format.",
    "Error: Key not found.",
    "Error: Unable to open file for saving data.",
    "Error: Failed to write data to file.",
    "Error: Unable to open file for loading data.",
    "Error: Invalid file format. Unable to parse key-value pairs.",
    "Error: Invalid command. Type 'help' for a list of available commands.",
    "Error: Failed to save data to file before exit.",
    "Error: An unexpected error occurred during exit. Data may not have been saved.",
    "Error: Incorrect usage of the command. Use 'help' to see the correct syntax.",
};

void	fn_putstr(const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		write(1, &str[i], 1);
}

void	fn_write_error(const char *error)
{
	if (!error) return ;
    while (*error)
    {
		write(2, error, 1);
		error++;
	}
	write(2, "\n", 1);
}

void    logger(int code)
{
    if (code >= 0 && code < 13)
		fn_write_error(error_messages[code]);
    else
		fn_write_error("Error.");
}

int	fn_strlen(const char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

char	*fn_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	while (i <= n)
	{
		dest[i] = '\0';
		i++;
	}
	return (dest);
}

int	fn_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

int count_words(char *str)
{
	int i;
	int j;
	int count;

	i = 0;
	j = 0;
	count = 0;
	while (str[i])
	{
		if (is_space(str[i]))
			j = 0;
		else if (j == 0)
		{
			count++;
			j = 1;
		}
		i++;
	}
	return (count);
}

void	read_file_into_buffer(const char *filename, char **content)
{
	int		fd;
	char	*temp;
	char	chunk[1024];
	ssize_t	bytes_read;
	size_t	buffer_size;
	size_t	buffer_capacity;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		fn_write_error("Error opening file");
		return ;
	}
	buffer_capacity = 1024;
	*content = (char *)malloc(buffer_capacity);
	if (*content == NULL)
	{
		fn_write_error("Memory allocation failed");
		close(fd);
		return ;
	}
	buffer_size = 0;
	while ((bytes_read = read(fd, chunk, sizeof(chunk))) > 0)
	{
		if (buffer_size + bytes_read + 1 > buffer_capacity)
		{
			buffer_capacity *= 2;
			temp = (char *)realloc(*content, buffer_capacity);
			if (!temp)
			{
				fn_write_error("Error reallocation memory");
				free(*content);
				close(fd);
				return ;
			}
			*content = temp;
		}
		memcpy(*content + buffer_size, chunk, bytes_read);
		buffer_size += bytes_read;
	}
	if (bytes_read < 0)
	{
		fn_write_error("Error reading file");
		free(*content);
		close(fd);
		return ;
	}
	// Check if the last character is a newline
	if (buffer_size > 0 && (*content)[buffer_size - 1] != '\n')
	{
		(*content)[buffer_size] = '\n';
		buffer_size++;
	}

	(*content)[buffer_size] = '\0';
	// (*content)[buffer_size] = '\n';
	// (*content)[buffer_size + 1] = '\0';
	if (close(fd) < 0)
		fn_write_error("Error closing file");
}