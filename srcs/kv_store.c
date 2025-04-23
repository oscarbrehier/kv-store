#include "kv_store.h"
#include "kv_table.h"
#include "libs.h"

t_status_code   create(char *name)
{
    t_kv_table      *table;
    t_status_code   ft_res;

    ft_res = kv_init_table(&table, 8);
    if (ft_res != SUCCESS_CODE)
        return (ft_res);
    ft_res = kv_save_file(table, construct_table_path(name, DIR_DATA));
    if (ft_res != SUCCESS_CODE)
        return (ft_res);
    return (SUCCESS_CODE);
}