#include "kv_table.h"
#include "libs.h"

t_status_code	kv_incr(t_kv_table *table, char *key)
{
	t_status_code	status;
	char			*current_value;
	char			*new_value;
    char			*endptr;
    long int		number;

	status = kv_get(table, key, (void *)&current_value, STRING);
	if (status != SUCCESS_CODE)
		return (status);
    number = strtol(current_value, &endptr, 10);
	if (endptr == current_value || *endptr != '\0')
		return (ERROR_STR_TO_INT_CODE);
	number++;
	new_value = ft_itoa(number);
	if (!new_value)
		return (ERROR_MEMORY_ALLOCATION_CODE);
	status = kv_set(table, key, new_value, ft_strlen(new_value), STRING);
	free(new_value);
	if (status != SUCCESS_CODE && status != WARNING_KEY_EXISTS_CODE)
		return (status);
	return (SUCCESS_CODE);
}

t_status_code	kv_decr(t_kv_table *table, char *key)
{
	t_status_code	status;
	char			*current_value;
	char			*new_value;
    char			*endptr;
    long int		number;

	status = kv_get(table, key, (void *)&current_value, STRING);
	if (status != SUCCESS_CODE)
		return (status);
    number = strtol(current_value, &endptr, 10);
	if (endptr == current_value || *endptr != '\0')
		return (ERROR_STR_TO_INT_CODE);
	number--;
	new_value = ft_itoa(number);
	if (!new_value)
		return (ERROR_MEMORY_ALLOCATION_CODE);
	status = kv_set(table, key, new_value, ft_strlen(new_value), STRING);
	free(new_value);
	if (status != SUCCESS_CODE && status != WARNING_KEY_EXISTS_CODE)
		return (status);
	return (SUCCESS_CODE);
}