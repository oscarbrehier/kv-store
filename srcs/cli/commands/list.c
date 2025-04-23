#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include "kv_store.h"
#include "libs.h"
#include "cli.h"

char	*get_store_name(char *dirname)
{
	int		len;
	char	ext[] = ".kvdb";
	char	*name;

	len = ft_strlen(dirname) - ft_strlen(ext);
	name = malloc(sizeof(char) * (len + 1));
	if (!name)
		return (dirname);
	strncpy(name, dirname, len);
	name[len] = '\0';
	return (name);
}

char	*get_mod_time(const char *filepath)
{
	struct stat	s_file;
	char		buf[80];

	memset(&s_file, 0, sizeof(s_file));
	if (stat(filepath, &s_file) == -1)
	{
		logger(1, "Error: Failed to get file propreties.");
		return (NULL);
	}
	formatTimestamp(s_file.st_mtime, buf, sizeof(buf));
	return (strdup(buf));
}

char	***realloc_data(char ***data, int size, int i)
{
	char	***temp;
	
	temp = realloc(data, sizeof(char **) * size);
	if (!temp)
	{
		logger(1, ERROR_MEMORY_REALLOCATION);
		cleanup_data_table(data, i);
		return (NULL);
	}
	return (temp);
}

static char	***get_data(int *rows)
{
	int				i;
	int				size;
	DIR 			*d;
    struct dirent	*dir;
	char			***data;
	char			file_path[263];

    d = opendir("./data");
	if (!d)
	{
		logger(1, "Error: Failed to open data dir.");
	}
	size = 2;
	data = malloc(sizeof(char **) * size);
	if (!data)
	{
		logger(1, ERROR_MEMORY_ALLOCATION);
		return (NULL);
	}
	i = 0;
    while ((dir = readdir(d)) != NULL) {
        if (ft_strcmp(dir->d_name, ".") == 0 || ft_strcmp(dir->d_name, "..") == 0)
            continue;
        if (strstr(dir->d_name, ".kvdb"))
        {
			if (i >= size)
			{
				size *= 2;
				data = realloc_data(data, size * 2, i);
				if (!data)
				{
					closedir(d);
					return (NULL);
				}
			}
			data[i] = malloc(sizeof(char *) * 3);
			if (!data[i])
			{
				logger(1, ERROR_MEMORY_ALLOCATION);
				cleanup_data_table(data, i);
				return (NULL);
			}
			data[i][0] = get_store_name(dir->d_name);
			snprintf(file_path, sizeof(file_path), "./data/%s", dir->d_name);
			data[i][1] = get_mod_time(file_path);
			if (!data[i][1])
			{
				free(data[i][1]);
				free(data[i]);
				data[i] = NULL;
				continue ;
			}
			data[i][2] = NULL;
			i++;
		}
    }
	if (i >= size)
	{
		data = realloc_data(data, size + 1, i);
		if (!data)
		{
			closedir(d);
			return (NULL);
		}
	}
	data[i] = NULL;
	*rows = i;
    closedir(d);
	return (data);
}

void    cmd_list(t_kv_store *store, int argc, char **argv)
{
    (void)store;
    (void)argc;
    (void)argv;
	int				row_count;
	char			***data;
	t_data_table	data_table;

	init_data_table(&data_table, "Stores", (const char *[]){"Name", "Last Modified", NULL});
	data = get_data(&row_count);
	if (!data)
		return ;
	data_table.data = data;
	display_table(data_table);
	cleanup_data_table(data, row_count);
}