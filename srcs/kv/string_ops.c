#include "kv_table.h"
#include "libs.h"

int	default_incr_value(char **value)
{
	*value = malloc(sizeof(char) * 2);
	if (!*value)
		return (1);
	(*value)[0] = '0';
	(*value)[1] = '\0';
	return (0);
}

t_status_code	kv_incr_by(t_kv_table *table, char *key, int increment)
{
	t_status_code	status;
	char			*current_value;
	char			*new_value;
    char			*endptr;
    long int		number;
	int				needs_free;

	status = kv_get(table, key, (void *)&current_value, STRING);
	needs_free = 0;
	if (status == ERROR_KEY_NOT_FOUND_CODE)
	{
		if (default_incr_value(&current_value))
			return (ERROR_MEMORY_ALLOCATION_CODE);
		needs_free = 1;
	}
	else if (status != SUCCESS_CODE)
		return (status);	
    number = strtol(current_value, &endptr, 10);
	if (endptr == current_value || *endptr != '\0')
		return (ERROR_STR_TO_INT_CODE);
	number += increment;
	new_value = ft_itoa(number);
	if (!new_value)
		return (ERROR_MEMORY_ALLOCATION_CODE);
	status = kv_set(table, key, new_value, ft_strlen(new_value), STRING);
	free(new_value);
	if (needs_free)
		free(current_value);
	if (status != SUCCESS_CODE && status != WARNING_KEY_EXISTS_CODE)
		return (status);
	return (SUCCESS_CODE);
}

t_status_code	kv_decr_by(t_kv_table *table, char *key, int increment)
{
	t_status_code	status;
	char			*current_value;
	char			*new_value;
    char			*endptr;
    long int		number;
	int				needs_free;

	status = kv_get(table, key, (void *)&current_value, STRING);
	needs_free = 0;
	if (status == ERROR_KEY_NOT_FOUND_CODE)
	{
		if (default_incr_value(&current_value))
			return (ERROR_MEMORY_ALLOCATION_CODE);
		needs_free = 1;
	}
	else if (status != SUCCESS_CODE)
		return (status);
    number = strtol(current_value, &endptr, 10);
	if (endptr == current_value || *endptr != '\0')
		return (ERROR_STR_TO_INT_CODE);
	number -= increment;
	new_value = ft_itoa(number);
	if (!new_value)
		return (ERROR_MEMORY_ALLOCATION_CODE);
	status = kv_set(table, key, new_value, ft_strlen(new_value), STRING);
	free(new_value);
	if (needs_free)
		free(current_value);
	if (status != SUCCESS_CODE && status != WARNING_KEY_EXISTS_CODE)
		return (status);
	return (SUCCESS_CODE);
}

t_status_code	kv_append(t_kv_table *table, char *key, char *value)
{
	t_status_code	status;
	int				len_old;
	int				len_val;
	char			*old;
	char			*new;

	status = kv_get(table, key, (void *)&old, STRING);
	if (status != SUCCESS_CODE)
		return (status);
	len_old = ft_strlen(old);
	len_val = ft_strlen(value);
	new = malloc(sizeof(char) * (len_old + len_val + 1));
	if (!new)
		return (ERROR_MEMORY_ALLOCATION_CODE);
	memcpy(new, old, len_old);
	memcpy(new + len_old, value, len_val);
	new[len_old + len_val] = '\0';
	status = kv_set(table, key, new, len_old + len_val, STRING);
	if (status != SUCCESS_CODE && status != WARNING_KEY_EXISTS_CODE)
	{
		free(new);
		return (status);
	}
	free(new);
	return (SUCCESS_CODE);
}

t_status_code	kv_strlen(t_kv_table *table, char *key, char **output)
{
	t_status_code	status;
	char			*value;
	int				value_len;

	status = kv_get(table, key, (void *)&value, STRING);
	if (status != SUCCESS_CODE)
		return (status);
	value_len = ft_strlen(value);
	*output = ft_itoa(value_len);
	if (*output == NULL)
		return (ERROR_MEMORY_ALLOCATION_CODE);
	return (SUCCESS_CODE);
}