#include <dirent.h>
#include "kv_store.h"
#include "libs.h"

void    table_list(void)
{
    DIR *d;
    struct dirent *dir;

    d = opendir("./data");
    if (!d)
    {
        logger(1, "Error: Failed to open data dir.");
    }
    while ((dir = readdir(d)) != NULL) {
        if (ft_strcmp(dir->d_name, ".") == 0 || ft_strcmp(dir->d_name, "..") == 0)
            continue;
        if (strstr(dir->d_name, ".kvdb"))
            printf("%s\n", dir->d_name);
    }
    closedir(d);
}