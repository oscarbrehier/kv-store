#include "kv_table.h"
#include "libs.h"

int	write_header(int fd)
{
	int header_len;

	header_len = ft_strlen(FILE_HEADER);
	if (write(fd, FILE_HEADER, header_len) != (ssize_t)header_len)
	{
		log_message(1, ERROR_FILE_WRITE_CODE);
		close(fd);
		return 0;
	}
	return (1);
}

t_status_code	kv_save_file(t_kv_table *table, const char *filename)
{
	int i;
	int fd;
	uint32_t key_len;
	uint32_t val_len;
	t_kv_pair *current;

	i = 0;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (ERROR_FILE_OPEN_CODE);
	if (!(write_header(fd)))
		return (ERROR_FILE_WRITE_CODE);
	while (i < table->capacity)
	{
		current = table->buckets[i];
		while (current != NULL)
		{
			key_len = ft_strlen(current->key);
			val_len = ft_strlen(current->value);
			if (write(fd, &key_len, sizeof(uint32_t)) != sizeof(uint32_t))
			{
				close(fd);
				return (ERROR_FILE_WRITE_CODE);
			}
			if (write(fd, current->key, key_len) != (ssize_t)key_len)
			{
				close(fd);
				return (ERROR_FILE_WRITE_CODE);
			}
			if (write(fd, &val_len, sizeof(uint32_t)) != sizeof(uint32_t))
			{
				close(fd);
				return (ERROR_FILE_WRITE_CODE);
			}
			if (write(fd, current->value, val_len) != (ssize_t)val_len)
			{
				close(fd);
				return (ERROR_FILE_WRITE_CODE);
			}
			current = current->next;
		}
		i++;
	}
	close(fd);
	return (SUCCESS_CODE);
};

t_status_code	kv_load_file(t_kv_table *table, const char *filename)
{
	int				fd;
	uint32_t		key_len;
	uint32_t		val_len;
	char			*key;
	char			*val;
	char			header[4];
	t_status_code	status;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (ERROR_FILE_READ_CODE);
	if (read(fd, header, 4) != 4 || memcmp(header, FILE_HEADER, 4) != 0)
	{
		close (fd);
		return (ERROR_FILE_HEADER_CODE);
	}
	while (read(fd, &key_len, sizeof(uint32_t)) == sizeof(uint32_t))
	{
		key = malloc(key_len + 1);
		if (!key)
		{
			close(fd);
			return (ERROR_MEMORY_ALLOCATION_CODE);
		}
		if (read(fd, key, key_len) != (ssize_t)key_len)
		{
			close(fd);
			return (ERROR_READ_KEY_CODE);
		}
		key[key_len] = '\0';
		if (read(fd, &val_len, sizeof(uint32_t)) != sizeof(uint32_t))
		{
			close(fd);
			return (ERROR_READ_VAL_LEN_CODE);
		}
		val = malloc(val_len + 1);
		if (!val)
		{
			close(fd);
			return (ERROR_MEMORY_ALLOCATION_CODE);
		}
		if (read(fd, val, val_len) != (ssize_t)val_len)
		{
			close(fd);
			return (ERROR_READ_VAL_CODE);
		}
		val[val_len] = '\0';
		status = kv_set(table, key, val);
		if (status != SUCCESS_CODE)
			return (status);
		free(key);
		free(val);
	}
	close(fd);
	return (SUCCESS_CODE);
}