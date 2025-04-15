#include "kv_store.h"

void	ft_putstr(int fd, const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		write(fd, &str[i], 1);
}

void    logger(int fd, char *content)
{
	ft_putstr(fd, content);
	ft_putstr(fd, "\n");
}

int	ft_strlen(const char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

char	*ft_strncpy(char *dest, char *src, unsigned int n)
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

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
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
		logger(1, ERROR_FILE_OPEN);
		return ;
	}
	buffer_capacity = 1024;
	*content = (char *)malloc(buffer_capacity);
	if (*content == NULL)
	{
		logger(1, ERROR_MEMORY_ALLOCATION);
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
				logger(1, "Error: Failed to reallocate memory.");
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
		logger(1, ERROR_FILE_READ);
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
		logger(1,ERROR_FILE_CLOSE);
}