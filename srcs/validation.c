#include "kv_store.h"

int is_alpha_num(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        return (1);
    return (0);
}

int	is_space(char c)
{
	if (c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

int is_valid_key(const char *key)
{
    int i;

    i = 0;
    while (key[i])
    {
        if (!is_alpha_num(key[i]) && key[i] != '_' && key[i] != '-' && key[i] != '.' && key[i] != ':')
			return (0);
        i++;
    }
    return (1);
}