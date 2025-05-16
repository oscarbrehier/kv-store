#include "kv_table.h"
#include "libs.h"

int	write_header(int fd)
{
	int header_len;

	header_len = ft_strlen(FILE_HEADER);
	if (write(fd, FILE_HEADER, header_len) != (ssize_t)header_len)
	{
		status_log(1, ERROR_FILE_CLOSE);
		close(fd);
		return 0;
	}
	return (1);
}

t_status	kv_save_file(t_kv_table *table, const char *filename)
{
	int 		i;
	int 		fd;
	uint32_t 	key_len;
	uint32_t 	val_len;
	t_kv_pair 	*current;
	t_status	status;

	pthread_rwlock_rdlock(&table->rwlock);

	i = 0;
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		status = status_create(-1, ERROR_FILE_OPEN, LOG_ERROR);
		return (status);
	}
	if (!(write_header(fd)))
	{
		status = status_create(-1, ERROR_FILE_WRITE, LOG_ERROR);
		return (status);
	}
	while (i < table->capacity)
	{
		current = table->buckets[i];
		while (current != NULL)
		{
			key_len = ft_strlen(current->key);
			val_len = current->value_size;
			if (write(fd, &key_len, sizeof(uint32_t)) != sizeof(uint32_t) ||
				write(fd, current->key, key_len) != (ssize_t)key_len ||
				write(fd, &val_len, sizeof(uint32_t)) != sizeof(uint32_t) ||
				write(fd, current->value, val_len) != (ssize_t)val_len)
			{
				close(fd);
				pthread_rwlock_unlock(&table->rwlock);
				status = status_create(-1, ERROR_FILE_WRITE, LOG_ERROR);
				return (status);
			}
			current = current->next;
		}
		i++;
	}
	close(fd);
	pthread_rwlock_unlock(&table->rwlock);
	status = status_create(0, SUCCESS, LOG_INFO);
	return (status);
};

t_status	kv_load_file(t_kv_table *table, const char *filename)
{
	int				fd;
	uint32_t		key_len;
	uint32_t		val_len;
	char			*key;
	char			*val;
	char			header[4];
	t_status		status;

	pthread_rwlock_wrlock(&table->rwlock);

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		pthread_rwlock_unlock(&table->rwlock);
		status = status_create(-1, ERROR_FILE_OPEN, LOG_ERROR);
		return (status);
	}
	if (read(fd, header, 4) != 4 || memcmp(header, FILE_HEADER, 4) != 0)
	{
		close (fd);
		pthread_rwlock_unlock(&table->rwlock);
		status = status_create(-1, ERROR_FILE_HEADER, LOG_ERROR);
		return (status);
	}
	while (read(fd, &key_len, sizeof(uint32_t)) == sizeof(uint32_t))
	{
		key = malloc(key_len + 1);
		if (!key)
		{
			close(fd);
			pthread_rwlock_unlock(&table->rwlock);
			status = status_create(-1, ERROR_MEMORY_ALLOCATION, LOG_ERROR);
			return (status);
		}
		if (read(fd, key, key_len) != (ssize_t)key_len)
		{
			close(fd);
			pthread_rwlock_unlock(&table->rwlock);
			status = status_create(-1, ERROR_READ_KEY, LOG_ERROR);
			return (status);
		}
		key[key_len] = '\0';
		if (read(fd, &val_len, sizeof(uint32_t)) != sizeof(uint32_t))
		{
			close(fd);
			pthread_rwlock_unlock(&table->rwlock);
			status = status_create(-1, ERROR_READ_VAL_LEN, LOG_ERROR);
			return (status);
		}
		val = malloc(val_len);
		if (!val)
		{
			close(fd);
			pthread_rwlock_unlock(&table->rwlock);
			status = status_create(-1, ERROR_MEMORY_ALLOCATION, LOG_ERROR);
			return (status);
		}
		if (read(fd, val, val_len) != (ssize_t)val_len)
		{
			close(fd);
			status = status_create(-1, ERROR_READ_VAL, LOG_ERROR);
			return (status);
		}
		status = _kv_set_internal(table, key, val, val_len, BINARY, 0);
		if (status.code != SUCCESS)
		{
			close(fd);
			pthread_rwlock_unlock(&table->rwlock);
			return (status);
		}
		free(key);
		free(val);
	}
	close(fd);
	pthread_rwlock_unlock(&table->rwlock);
	status = status_create(0, SUCCESS, LOG_INFO);
	return (status);
}