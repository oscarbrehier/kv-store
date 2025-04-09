#include "kv_store.h"

int kv_formatted_length(const char *key, const char *value)
{
	return (fn_strlen(key) + fn_strlen(value) + 3);
}

char *kv_format_str(const char *key, const char *value)
{
	char *str;
	char *ptr;
	size_t len;
	size_t len_key;
	size_t len_value;

	len = kv_formatted_length(key, value) + 1;
	len_key = fn_strlen(key);
	len_value = fn_strlen(value);
	str = malloc(sizeof(char) * len);
	if (!str)
		return (NULL);
	ptr = str;
	ptr = memcpy(ptr, key, len_key) + len_key;
	ptr = memcpy(ptr, ": ", 2) + 2;
	ptr = memcpy(ptr, value, len_value) + len_value;
	*ptr = '\n';
	ptr++;
	*ptr = '\0';
	return (str);
}

void kv_save_to_file(t_kv_pair **table, const char *filename)
{
	int i;
	int fd;
	ssize_t length;
	t_kv_pair *current;
	char *content;

	i = 0;
	fd = open(filename, O_WRONLY | O_CREAT, 0644);
	if (fd < 0)
	{
		fn_write_error("Error opening file");
		return;
	}
	while (i < TABLE_SIZE)
	{
		current = table[i];
		while (current != NULL)
		{
			content = kv_format_str(current->key, current->value);
			if (!content)
			{
				fn_write_error("Memory allocation failed");
				close(fd);
				return;
			}
			length = fn_strlen(content);
			if (write(fd, content, length) != length)
			{
				fn_write_error("Error writing to file");
				free(content);
				close(fd);
				return;
			}
			free(content);
			current = current->next;
		}
		i++;
	}
	close(fd);
};

int get_line_length(const char *content, int start)
{
	int len;
	int i;

	len = 0;
	i = start;
	while (content[i] && content[i] != '\n')
	{
		len++;
		i++;
	}
	return (len);
}

int get_key_len(const char *fc, int index)
{
	int i;
	int len;

	i = index;
	len = 0;
	while (fc[i] && fc[i] != ':' && fc[i] != '\n')
	{
		if ((fc[i] & 0xC0) != 0x80)
			len++;
		i++;
	}
	return (len);
}

int get_value_len(const char *fc, int index)	
{
	int i;
	int len;

	i = index;
	len = 0;
	if (!fc)
		return (0);
	while (fc[i] != '\0' && fc[i] != '\n')
	{
		if ((fc[i] & 0xC0) != 0x80)
			len++;
		i++;
	}
	return (len);
}

int is_valid_kv_line(char *fc, int index)
{
    int i;

    i = index;

    // No spaces allowed before the key
    if (is_space(fc[i]))
		return (0);

    // If the key is empty or there is a colon right away, it's invalid
    if (fc[i] == ':' || fc[i] == '\0' || fc[i] == '\n') {
        return 0;
    }

    // Move until you hit the colon, if space in key return 0
    while (fc[i] && fc[i] != ':' && fc[i] != '\n') {
        if (is_space(fc[i]))
			return (0);
		i++;
    }

    // If no colon is found, it's an invalid line
    if (fc[i] != ':') {
        return 0;
    }

    // Ensure no spaces before colon in the key
    if (i == index) {
        return 0;
    }

    // Skip spaces after the colon
    i++;
    while (fc[i] == ' ') i++;

    // If there's no value after the colon, it's invalid
    if (fc[i] == '\0' || fc[i] == '\n') {
        return 0;
    }

    // If we reached here, the line is valid
    return 1;
}

int parse_kv_from_line(char *fc, int index, char **key, char **value)
{
	int key_len;
	int val_len;

	key_len = get_key_len(fc, index);
	val_len = get_value_len(fc, index + key_len + 1);

	*key = malloc(sizeof(char) * (key_len + 2));
	*value = malloc(sizeof(char) * (val_len + 2));
	if (!*key || !*value || !is_valid_kv_line(fc, index))
	{
		free(*key);
		free(*value);
		return (1);
	}
	int i = index;
	int	j = 0;
	while (fc[i] && fc[i] != ':' && fc[i] != '\n')
	{
		(*key)[j] = fc[i];
		j++;
		i++;
	}
	(*key)[j] = '\0';
	i++;
	j = 0;
	while (fc[i] && fc[i] != '\n')
	{
		(*value)[j] = fc[i];
		j++;
		i++;
	}
	(*value)[j] = '\0';
	return (0);
}

void kv_load_from_file(t_kv_pair **table, const char *filename)
{
	char *fc;
	int i;
	char *key = NULL;
	char *value = NULL;

	read_file_into_buffer(filename, &fc);
	if (fc == NULL)
	{
		fn_write_error("Failed to read file into buffer.\n");
		return;
	}
	i = 0;
	while (fc[i])
	{
		if (parse_kv_from_line(fc, i, &key, &value) == 0)
		{
			kv_set(table, key, value);
			free(key);
			free(value);
		}
		i += get_line_length(fc, i);
		if (fc[i] == '\n')
			i++;
	}
	free(fc);
}
