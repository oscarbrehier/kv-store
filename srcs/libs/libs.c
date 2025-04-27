#include "utils.h"
#include "kv_store.h"
#include "libs.h"
#include "status_codes.h"

void	ft_putstr(int fd, const char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		write(fd, &str[i], 1);
}

int	calculate_formatted_len(char *content, va_list args_cpy, int format_count)
{
	int		i;
	int		len;
	int		arg_count;
	char 	*arg;
	va_list	args;

	i = 0;
	va_copy(args, args_cpy);
	i = 0;
	len = 0;
	arg_count = 0;
	while (content[i])
	{
		if (content[i] == '%' && content[i + 1] == 's')
		{
			if (arg_count < format_count)
			{
				arg = va_arg(args, char *);
				arg_count++;
				len += arg ? ft_strlen(arg) : 6;
			}
			else
			{
				len += 8;
			}
			i += 2;
		}
		else
		{
			len++;
			i++;
		}
	}
	va_end(args);
	return (len);
}

void    logger(int fd, char *content, ...)
{
	int		i;
	int		j;
	int 	arg_len;
	int		strlen;
	va_list args;
	va_list args_copy;
	char	*formatted_content;
	char	*arg;
	int		format_count = 0;
	int		arg_count = 0;
	char	*missing_str = "(missing)";
	char	*null_str = "(null)";

	i = 0;
	while (content[i])
	{
		if (content[i] == '%' && content[i + 1] == 's')
			format_count++;
		i++;
	}

	va_start(args, content);
	va_copy(args_copy, args);
	strlen = calculate_formatted_len(content, args_copy, format_count);
	va_end(args_copy);

	// Removed check for strlen == 0 as it might reject valid empty strings
	// if (strlen == 0)
	// {
	// 	ft_putstr(fd, content);
	// 	ft_putstr(fd, "\n");
	// 	return ;
	// }
	
	formatted_content = malloc(sizeof(char) * (strlen + 1));
	if (!formatted_content)
	{
		ft_putstr(fd, content);
		ft_putstr(fd, "\n");
		va_end(args);
		return ;
	}
	i = 0;
	j = 0;
	while (content[i])
	{
		if (content[i] == '%' && content[i + 1] == 's')
		{
			if (arg_count < format_count)
			{
				arg = va_arg(args, char *);
				if (arg)
				{
					arg_len = ft_strlen(arg);
					memcpy(&formatted_content[j], arg, arg_len);
					arg_count++;
					j += arg_len;
				}
				else
				{
					memcpy(&formatted_content[j], null_str, 6);
					j += 6;
				}
			}
			else
			{
				memcpy(&formatted_content[j], missing_str, 9);
				j += 8;
			}
			i += 2;
		}
		else
		{
			formatted_content[j++] = content[i++];
		}
	}
	formatted_content[j] = '\0';
	
	printf("%s\n", formatted_content);
	free(formatted_content);
	va_end(args);
}

int	ft_strlen(char *str)
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
		log_message(1, ERROR_FILE_OPEN_CODE);
		return ;
	}
	buffer_capacity = 1024;
	*content = (char *)malloc(buffer_capacity);
	if (*content == NULL)
	{
		log_message(1, ERROR_MEMORY_ALLOCATION_CODE);
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
		log_message(1, ERROR_FILE_READ_CODE);
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
		log_message(1, ERROR_FILE_CLOSE_CODE);
}

void formatTimestamp(time_t timestamp, char *buf, size_t buf_size)
{
	struct tm	ts;
	
	ts = *localtime(&timestamp);
	strftime(buf, buf_size, "%a %Y-%m-%d %H:%M:%S %Z", &ts);
}

t_status_code	construct_table_path(char *filename, char *path, const char **output)
{
	char	ext[] = ".kvdb";
	char	*fullpath;

	fullpath = malloc(sizeof(char) * (ft_strlen(path) + ft_strlen(filename) + ft_strlen(ext) + 1));
	if (!fullpath)
	{
		return (ERROR_MEMORY_ALLOCATION_CODE);
	}
	sprintf(fullpath, "%s%s%s", path, filename, ext);
	*output = fullpath;
	return (SUCCESS_CODE);
} 

int	intlen(long int nbr)
{
	int	len;

	len = 1;
	while (nbr / 10)
		len++;
	return (len);
}

char	*ft_itoa(int n)
{
	long int	nbr;
	int			len;
	char		*str;

	nbr = n;
	len = intlen(nbr);
	if (n < 0)
		len++;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (nbr < 0)
		nbr = -nbr;
	while (len--)
	{
		str[len] = nbr % 10 + '0';
		nbr /= 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}