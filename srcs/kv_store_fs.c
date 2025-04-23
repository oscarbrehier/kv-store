#include "kv_store.h"
#include "libs.h"

int	write_header(int fd)
{
	int header_len;

	header_len = ft_strlen(FILE_HEADER);
	if (write(fd, FILE_HEADER, header_len) != (ssize_t)header_len)
	{
		logger(1, ERROR_FILE_WRITE);
		close(fd);
		return 0;
	}
	return (1);
}

void	kv_save_file(t_kv_table *table, const char *filename)
{
	int i;
	int fd;
	uint32_t key_len;
	uint32_t val_len;
	t_kv_pair *current;
	// char *content;

	i = 0;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		logger(1, ERROR_FILE_OPEN);
		return;
	}
	if (!(write_header(fd)))
		return ;
	while (i < table->capacity)
	{
		current = table->buckets[i];
		while (current != NULL)
		{
			key_len = ft_strlen(current->key);
			val_len = ft_strlen(current->value);
			if (write(fd, &key_len, sizeof(uint32_t)) != sizeof(uint32_t))
			{
				logger(1, ERROR_FILE_WRITE);
				close(fd);
				return;
			}
			if (write(fd, current->key, key_len) != (ssize_t)key_len)
			{
				logger(1, ERROR_FILE_WRITE);
				close(fd);
				return;
			}
			if (write(fd, &val_len, sizeof(uint32_t)) != sizeof(uint32_t))
			{
				logger(1, ERROR_FILE_WRITE);
				close(fd);
				return;
			}
			if (write(fd, current->value, val_len) != (ssize_t)val_len)
			{
				logger(1, ERROR_FILE_WRITE);
				close(fd);
				return;
			}
			// free(content);
			current = current->next;
		}
		i++;
	}
	logger(2, SUCCESS);
	close(fd);
};

void	kv_load_file(t_kv_table *table, const char *filename)
{
	int			fd;
	uint32_t	key_len;
	uint32_t	val_len;
	char		*key;
	char		*val;
	char		header[4];
	t_status_code	result;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		logger(1, ERROR_FILE_READ);
		return ;
	}
	if (read(fd, header, 4) != 4 || memcmp(header, FILE_HEADER, 4) != 0)
	{
		logger(1, ERROR_FILE_HEADER);
		close (fd);
		return ;
	}
	while (read(fd, &key_len, sizeof(uint32_t)) == sizeof(uint32_t))
	{
		key = malloc(key_len + 1);
		if (!key)
		{
			logger(1, ERROR_MEMORY_ALLOCATION);
			close(fd);
			return ;
		}
		if (read(fd, key, key_len) != (ssize_t)key_len)
		{
			logger(1, "Error : Failed to read key from file.");
			close(fd);
			return ;
		}
		key[key_len] = '\0';
		if (read(fd, &val_len, sizeof(uint32_t)) != sizeof(uint32_t))
		{
			logger(1, "Error: Failed to read value length from file.");
			close(fd);
			return ;
		}
		val = malloc(val_len + 1);
		if (!val)
		{
			logger(1, ERROR_MEMORY_ALLOCATION);
			close(fd);
			return ;
		}
		if (read(fd, val, val_len) != (ssize_t)val_len)
		{
			logger(1, "Error: Failed to read value from file.");
			close(fd);
			return ;
		}
		val[val_len] = '\0';
		result = kv_set(table, key, val);
		if (result != SUCCESS_CODE)
		{
			log_message(1, result);
		}
		free(key);
		free(val);
	}
	close(fd);
	log_message(2, SUCCESS_CODE);
}